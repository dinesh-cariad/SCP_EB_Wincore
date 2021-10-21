/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 2.1 (required)
 *    A project shall not contain unreachable code.
 *
 *    Reason:
 *    In case of a single network macros to access the configuration are directly translated to
 *    boolean values which can lead to contraditing condition statments (e.g. TRUE == FALSE).
 *
 *  MISRAC2012-2) Deviated Rule: 14.3 (required)
 *    controlling expressions shall no be invariant.
 *
 *    Reason:
 *    In case of a single network macros to access the configuration are directly translated to
 *    boolean values which can lead to contraditing condition statments (e.g. TRUE == FALSE).
 */

/*==================[inclusions]============================================*/

#include <CanSM_Trace.h>
#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* AUTOSAR communication stack types */
#include <TSAtomic_Assign.h>        /* Atomic assignment macros */

#include <CanSM_Cfg.h>              /* Generated configuration */
#include <CanSM_Internal.h>
#include <CanSM_InternalCfg.h>      /* BSWM_CANSM_CURRENTSTATE */
#include <CanSM_BswM.h>

#include <CanIf.h>                  /* CAN Interface */
#include <ComM_BusSm.h>             /* ComM BusSm API */
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                    /* Development Error Tracer */
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

/* -----------------[ Transitions ]---------------------------------------- */

/**
 * \brief Dispatches the actual transition.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_DoTransitionSwitch(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition,
  const boolean invokedInMainFunction
);

/**
 * \brief Dispatches the actual transition for main state Full Communication.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Dispatches the actual transition for main state Request No Communication.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Dispatches the actual transition for main state Request Full Communication.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition,
  const boolean invokedInMainFunction
);

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
/**
 * \brief Dispatches the actual transition for main state Partial Network.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

#if (CANSM_SET_BAUDRATE_API == STD_ON)
/**
 * \brief Dispatches the actual transition for main state Change Baudrate.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* CANSM_SET_BAUDRATE_API */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
/**
 * \brief Dispatches the actual transition for main state Tx Timeout Exception.
 *
 * \param[in] nw Network index
 * \param[in] transition Enum value identifying transition
 * \param[in,out] nextTransition If another transition is triggered by the dispatched transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

/* transitions to NOCO */

/**
 * \brief Executes transition CANSM_T_RNOCO_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RNOCO_DEINIT_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_DEINIT_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RNOCO_CC_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RNOCO_CC_STOPPED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_STOPPED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RNOCO_CC_SLEEP.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_SLEEP(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#if (CANSM_USE_TRANSCEIVER == STD_ON)

/**
 * \brief Executes transition CANSM_T_RNOCO_TRCV_NORMAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_TRCV_NORMAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

/**
 * \brief Executes transition CANSM_T_RNOCO_FINAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_FINAL(
  const CanSM_NetworkIndexType nw
);

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
/* transitions to NOCO with partial networking */

/**
 * \brief Executes transition CANSM_T_PN_DEINIT.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_DEINIT(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_CLEAR_WUF_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CLEAR_WUF_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_CC_STOPPED_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_TRCV_NORMAL_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_TRCV_NORMAL_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_TRCV_STANDBY_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_TRCV_STANDBY_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_CC_SLEEP_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_SLEEP_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_PN_CC_SLEEP_TIMEOUT.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_SLEEP_TIMEOUT(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

/* transitions to FUCO */

/**
 * \brief Executes transition CANSM_T_RFUCO_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#if (CANSM_USE_TRANSCEIVER == STD_ON)

/**
 * \brief Executes transition CANSM_T_RFUCO_TRCV_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_TRCV_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

/**
 * \brief Executes transition CANSM_T_RFUCO_TRCV_NORMAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_TRCV_NORMAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RFUCO_CC_STOPPED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_CC_STOPPED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_RFUCO_CC_STARTED.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_CC_STARTED(
  const CanSM_NetworkIndexType nw,
  const boolean invokedInMainFunction
);

/* transitions between FUCO and SICO */

/**
 * \brief Executes transition CANSM_T_FUCO_SICO.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_SICO(
  const CanSM_NetworkIndexType nw
);

/**
 * \brief Executes transition CANSM_T_SICO_FUCO.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_SICO_FUCO(
  const CanSM_NetworkIndexType nw,
  const boolean invokedInMainFunction
);

#if (CANSM_SET_BAUDRATE_API == STD_ON)

/**
 * \brief Executes transition CANSM_T_FUCO_CHBR.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_CHBR(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);


/**
 * \brief Executes transition CANSM_T_CHBR_CC_STOP.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_CC_STOP(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_CHBR_STOPPED_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_CHBR_CC_START.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_CC_START(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_CHBR_STARTED_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in] invokedInMainFunction TRUE if and only if transition executes in main function
 * context
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_STARTED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#endif /* CANSM_SET_BAUDRATE_API */

/* bus-off recovery state machine */

/**
 * \brief Executes transition CANSM_T_FUCO_HANDLE_BUS_OFF.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_HANDLE_BUS_OFF(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_FUCO_TX_OFF.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_TX_OFF(
  const CanSM_NetworkIndexType nw
);

#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
/**
 * \brief Executes transition CANSM_T_FUCO_OFF.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_OFF(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY.
 *
 * \param[in] nw             Network index
 *            nextTransition If another transition is triggered by this transition,
 *                           the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CANSM_Do_T_FUCO_BUS_DEACTIVATED_RECOVERY(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON) */

/**
 * \brief Executes transition CANSM_T_FUCO_TX_ON.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_TX_ON(
  const CanSM_NetworkIndexType nw
);

/**
 * \brief Executes transition T_FUCO_BUS_OFF_PASSIVE, unless there is a concurrent transition to silent
 * communication.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_TRY_T_FUCO_BUS_OFF_PASSIVE(
  const CanSM_NetworkIndexType nw
);

/* TxTimeout Exception transitions */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
/**
 * \brief Executes transition CANSM_T_TX_TIMEOUT_INITIAL.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED.
 *
 * \param[in] nw Network index
 * \param[in,out] nextTransition If another transition is triggered by this transition,
 * the function sets this parameter accordingly
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_CC_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Executes transition CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED.
 *
 * \param[in] nw Network index
 */
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_CC_STARTED_INDICATED(
  const CanSM_NetworkIndexType nw
);
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

/* CanSM_MainFunctionInternal subroutines */

/**
 * \brief Subroutine of CanSM_MainFunctionInternal.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Subroutine of CanSM_MainFunctionCheckModeIndications for state Request No Communication.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
/**
 * \brief Subroutine of CanSM_MainFunctionCheckModeIndications for state Partial Network
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

/**
 * \brief Subroutine of CanSM_MainFunctionCheckModeIndications for state Request Full Communication
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Subroutine of CanSM_MainFunctionInternal.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/**
 * \brief Subroutine of CanSM_MainFunctionRepeatModeRequests for state Request No Communication.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
/**
 * \brief Subroutine of CanSM_MainFunctionRepeatModeRequests for state Partial Network.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

/**
 * \brief Subroutine of CanSM_MainFunctionRepeatModeRequests for state Request Full Communication.
 *
 * \param[in] nw Network index
 * \param[in] internalState Copy of CanSM_MainFunctionInternal local variable
 * \param[in,out] nextTransition If a transition is triggered, the subroutine sets the parameter to
 * the corresponding value
 */
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
);

/* default trailing critical section */

/**
 * \brief Inside a critical section, sets CANSM_NW_INFO_INTERNALSTATE to newInternalState
 *
 * \param[in] nw Network index
 * \param[in] newInternalState New state for CANSM_NW_INFO_INTERNALSTATE
 */
STATIC FUNC(void, CANSM_CODE) CanSM_DefaultTrailingCriticalSection(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType newInternalState
);

/* controller helper functions */

/**
 * \brief Call CanIf_SetPduMode with the passed mode request for each controller.
 *
 * \param[in] nw Network index
 * \param[in] pduModeRequest Mode request
 */
STATIC FUNC(void, CANSM_CODE) CanSM_SetPduModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_PduModeType pduModeRequest
);

/**
 * \brief Clears the received mode indication for each controller and then calls
 * CanSM_SetCtrlModes with the passed mode request.
 *
 * \param[in] nw Network index
 * \param[in] ctrlMode Mode request
 * \param[out] allModeIndicationsReceived TRUE if and only if since the last last controller mode
 * indication reset, mode indications for ctrlMode have been received for all controllers
 * (and have not been superseded by other controller mode indications).
 */
STATIC FUNC(void, CANSM_CODE) CanSM_ClearCtrlModeIndAndSetModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) allModeIndicationsReceived
);

/**
 * \brief Checks if all expected controller mode indications have been received.
 *
 * \param[in] nw Network index
 * \param[in] ctrlMode Expected controller mode
 * \return boolean
 * \retval TRUE Since the last last controller mode indication reset, mode
 * indications for ctrlMode have been received for all controllers (and have not been
 * superseded by other controller mode indications).
 * \retval FALSE Else.
 */
STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckCtrlModeIndications(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode
);

/**
 * \brief For each controller for which no corresponding mode indication has been received since
 * the last controller mode indication reset:
 * Call CanIf_SetControllerMode with ctrlMode.
 *
 * \param[in] nw Network index
 * \param[in] ctrlMode Mode request
 * \param[out] allModeIndicationsReceived TRUE if and only if since the last last controller mode
 * indication reset, mode indications for ctrlMode have been received for all controllers
 * (and have not been superseded by other controller mode indications).
 */
STATIC FUNC(void, CANSM_CODE) CanSM_SetCtrlModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) allModeIndicationsReceived
);

/**
 * \brief Converts CanIf_ControllerModeType to CanSM specific representation.
 *
 * \param[in] ctrlMode Value passed to CanSM_ControllerModeIndication
 * \return CanSM specific representation
 */
STATIC FUNC(uint8, CANSM_CODE) CanSM_ControllerModeTypeToUint8(
  const CanIf_ControllerModeType ctrlMode
);

#if (CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON)
/**
 * \brief Checks if CanIf_GetTxConfirmationState returns CANIF_TX_RX_NOTIFICATION for all controllers of
 * the network.
 *
 * \param[in] nw Network index
 *
 * \return boolean
 * \retval TRUE All CanIf_GetTxConfirmationState returned CANIF_TX_RX_NOTIFICATION.
 * \retval FALSE Else.
 */
STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckCanIfGetTxConfirmationStates(
  const CanSM_NetworkIndexType nw
);
#endif /* CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON */

/* transceiver helper functions */

#if (CANSM_USE_TRANSCEIVER == STD_ON)

/**
 * \brief Clears the transceiver mode indication and then calls CanSM_SetTrcvMode with the
 * passed mode request.
 *
 * \param[in] nw Network index
 * \param[in] trcvMode Mode request
 * \param[out] modeIndicationReceived TRUE if and only if since the last transceiver mode
 * indication reset, the trcvMode mode indication has been received
 * (and has not been superseded by other transceiver mode indications).
 */
STATIC FUNC(void, CANSM_CODE) CanSM_ClearTrcvModeIndAndSetMode(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) modeIndicationReceived
);

/**
 * \brief Checks if the expected transceiver mode indication has been received.
 *
 * \param[in] nw Network index
 * \param[in] trcvMode Expected transceiver mode
 * \return boolean
 * \retval TRUE Since the last last transceiver mode indication reset, the trcvMode mode
 * indication has been received (and has not been superseded by other transceiver mode
 * indications).
 * \retval FALSE Else.
 */
STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckTrcvModeIndication(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode
);

/**
 * \brief Calls CanIf_SetTrcvMode with the passed mode request.
 *
 * \param[in] nw Network index
 * \param[in] trcvMode Mode request
 * \param[out] modeIndicationReceived TRUE if and only if since the last transceiver mode
 * indication reset, the trcvMode mode indication has been received (and has not been
 * superseded by other transceiver mode indications).
 */
STATIC FUNC(void, CANSM_CODE) CanSM_SetTrcvMode(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) modeIndicationReceived
);

/**
 * \brief Converts CanTrcv_TrcvModeType to CanSM specific representation.
 *
 * \param[in] trcvMode
 * \return CanSM specific representation
 */
STATIC FUNC(uint8, CANSM_CODE) CanSM_TrcvModeTypeToUint8(
  const CanTrcv_TrcvModeType trcvMode
);

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

/* partial networking helper functions */

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )

/**
 * \brief Calls CanIf_ClearTrcvWufFlag.
 *
 * \param[in] nw Network index
 * \param[out] clearIndicationReceived TRUE if and only if since the last
 * indication reset, the ClearTrcvWufFlag indication has been received.
 */
STATIC FUNC(void, CANSM_CODE) CanSM_ClearTrcvWufFlag(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) clearIndicationReceived
);

/**
 * \brief Calls CanIf_CheckTrcvWufFlag.
 *
 * \param[in] nw Network index
 * \param[out] checkIndicationReceived TRUE if and only if since the last
 * indication reset, the CheckTrcvWakeFlag indication has been received.
 */
STATIC FUNC(void, CANSM_CODE) CanSM_CheckTrcvWakeFlag (
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) checkIndicationReceived
);

#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

/**
 * \brief Calls CanSM_DecrementTimerInternal.
 *
 * \param[in] timer Network timer
 * \param[out] timer decremented if timer is grater than 0 OR
               timer             if timer is 0
 */
STATIC FUNC(void, CANSM_CODE) CanSM_DecrementTimerInternal (
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) timer
);

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(boolean, CANSM_CODE) CanSM_IsWaitingForModeIndication(
    const CanSM_InternalStateType state
)
{
  boolean result = FALSE;

  switch (state)
  {
#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RNOCO_CAN_TRCV_NORMAL:
    case CANSM_S_RNOCO_CAN_TRCV_STANDBY:
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */
    case CANSM_S_RNOCO_CAN_CC_STOPPED:
    case CANSM_S_RNOCO_CAN_CC_SLEEP:
#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
    case CANSM_S_PN_CLEAR_WUF:
    case CANSM_S_PN_CC_STOPPED:
    case CANSM_S_PN_TRCV_NORMAL:
    case CANSM_S_PN_TRCV_STANDBY:
    case CANSM_S_PN_CC_SLEEP:
    case CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP:
    case CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP:
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */
#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RFUCO_CAN_TRCV_NORMAL:
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */
    case CANSM_S_RFUCO_CAN_CC_STOPPED:
    case CANSM_S_RFUCO_CAN_CC_STARTED:
    case CANSM_S_FUCO_RESTART_CC:
#if (CANSM_SET_BAUDRATE_API == STD_ON)
    case CANSM_S_CHBR_CAN_CC_STOPPED:
    case CANSM_S_CHBR_CAN_CC_STARTED:
#endif /* CANSM_SET_BAUDRATE_API */
#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
    case CANSM_S_TX_TIMEOUT_CC_STOPPED:
    case CANSM_S_TX_TIMEOUT_CC_STARTED:
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */
      result = TRUE;
      break;
    case CANSM_S_NOIN:
    case CANSM_S_NOCO:
    case CANSM_S_SICO:
    case CANSM_S_FUCO_BUS_OFF_CHECK:
    case CANSM_S_FUCO_NO_BUS_OFF:
    case CANSM_S_FUCO_TX_OFF:
      /* do nothing */
      break;
    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return result;
}

FUNC(void, CANSM_CODE) CanSM_MainFunctionInternal(
  const CanSM_NetworkIndexType nw
)
{
  /* by default, don't trigger transition to another state */
  CanSM_TransitionType nextTransition = CANSM_T_NONE;
  /* flag for running through MainFunction transition logic */
  boolean DoMainFunctionProcessing = FALSE;

  CANSM_ENTER_CRITICAL_SECTION();
  {
    /* caches if a transition is already ongoing */
    boolean TransitionOngoing = CANSM_NW_INFO_TRANSITION_ONGOING(nw);

    /* Data Protection in MainFunction works in the following way:
       -- check if there is a synchronous transition ongoing ---
       a.) if there is a synchronous transition ongoing, then the
           MainFunction must not interrupt this transition.
           Do nothing in this cycle then...
       b1.) if there is no synchronous transition ongoing, then the
            MainFunction locks synchronous transitions by itself.
            This ensures data consistency if the MainFunction gets
            interrupted by CanSM_RequestComMode(9, CanSM_TxTimeoutException()
            or CanSM_SetBaudrate().
       b2.) check for transitions to be driven by the MainFunction
       b3.) unlock synchronous transition flag
    */
    if (TransitionOngoing == FALSE)
    {
      /* b1.) lock synchronous transitions */
      CANSM_NW_INFO_TRANSITION_ONGOING(nw) = TRUE;
      DoMainFunctionProcessing = TRUE;
    }
  }
  CANSM_EXIT_CRITICAL_SECTION();


  /* b2.) check for transitions to be driven by the MainFunction */
  if (DoMainFunctionProcessing == TRUE)
  {
    /* caches CANSM_NW_INFO_INTERNALSTATE(nw) */
    CanSM_InternalStateType internalState = CANSM_NW_INFO_INTERNALSTATE(nw);
    /* caches CANSM_NW_INFO_REQCOMMODE(nw) */
    ComM_ModeType reqMode = CANSM_NW_INFO_REQCOMMODE(nw);
    /* caches CANSM_NW_INFO_TIMER(nw) */
    uint16 nwInfoTimer;

    CanSM_DecrementTimerInternal(&CANSM_NW_INFO_TIMER(nw));

    nwInfoTimer = CANSM_NW_INFO_TIMER(nw);

    /* Check if the requested mode leads to a transition */
    CanSM_VerifyRequestedComMode( reqMode, internalState, &nextTransition );

    if ( nextTransition == CANSM_T_NONE )
    {
      /* caches CANSM_NW_INFO_BUSOFF(nw) */
      uint8 busOff;
      /* caches if we are waiting for a mode indication */
      boolean isWaitingForModeIndication = CanSM_IsWaitingForModeIndication(internalState);

      TS_AtomicAssign8(busOff, CANSM_NW_INFO_BUSOFF(nw));

      if (
           (busOff == CANSM_BUS_OFF)
           &&
           /* FUCO and not waiting for mode indication */
           (
               (internalState == CANSM_S_FUCO_BUS_OFF_CHECK)
               ||
               (internalState == CANSM_S_FUCO_NO_BUS_OFF)
             )
           )
      {
        /*
         * since CanSM_RequestComMode also checks bus-off flag within critical section,
         * we can be sure it denies a request to leave FUCO if a bus-off is detected here
         */
        nextTransition = CANSM_T_FUCO_HANDLE_BUS_OFF;
      }
      else if (internalState == CANSM_S_FUCO_TX_OFF)
      {
        if (nwInfoTimer == 0U)
        {
          nextTransition = CANSM_T_FUCO_TX_ON;
        }
      }
#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
      else if (internalState == CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY)
      {
        nextTransition = CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY;
      }
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */
      else if (internalState == CANSM_S_FUCO_BUS_OFF_CHECK)
      {
#if (CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON)
        /* Deviation MISRAC2012-2 */
        if (CANSM_BOR_TX_CONFIRMATION_POLLING(nw) == TRUE)
        {
          if (CanSM_CheckCanIfGetTxConfirmationStates(nw) == TRUE)
          {
            nextTransition = CANSM_T_FUCO_BUS_OFF_PASSIVE;
          }
        }
#if (CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON)
        else
#endif /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
#endif /* CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON */
#if (CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON)
        {
          if (nwInfoTimer == 0U)
          {
             nextTransition = CANSM_T_FUCO_BUS_OFF_PASSIVE;
          }
        }
#endif /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
      }
      else if (isWaitingForModeIndication == TRUE)
      {
        /* accesses to most shared data are safe, since there is a pending mode request */

        CanSM_MainFunctionCheckModeIndications(nw, internalState, &nextTransition);

        /* if mode indications are still pending */
        if (nextTransition == CANSM_T_NONE)
        {

          CanSM_DecrementTimerInternal(&CANSM_NW_INFO_MODE_REQUEST_REPETITION_TIMER(nw));

          /* check if mode request repetition timer fires */
          if (CANSM_NW_INFO_MODE_REQUEST_REPETITION_TIMER(nw) == 0U)
          {
            if (CANSM_NW_INFO_MODE_REQUEST_REPETITION_COUNTER(nw) == 0U)
            {
              /* maximum repetition count exceeded, begin transitions to NOCO */

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
              CANSM_DET_WITH_INSTANCEID(
                  CANSM_NW_HANDLE_FROM_IDX(nw),
                  CANSM_SID_MAINFUNCTION,
                  CANSM_E_MODE_REQUEST_TIMEOUT
              );
#endif /* CANSM_DEV_ERROR_DETECT == STD_ON */

              switch (internalState)
              {
                case CANSM_S_FUCO_RESTART_CC:
#if (CANSM_SET_BAUDRATE_API == STD_ON)
                case CANSM_S_CHBR_CAN_CC_STOPPED:
                case CANSM_S_CHBR_CAN_CC_STARTED:
#endif /* CANSM_SET_BAUDRATE_API == STD_ON */
#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
                case CANSM_S_TX_TIMEOUT_CC_STOPPED:
                case CANSM_S_TX_TIMEOUT_CC_STARTED:
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */
                  /* If CANSM_BSM_S_PRE_NOCOM shall be entered from CANSM_BSM_S_FULLCOM
                   * execute effect E_PRE_NO_COM (see SWS Figure 7-1). */
                  nextTransition = CANSM_T_RNOCO_INITIAL;
                  break;
                default:
                  /* If actual state is already a substate of CANSM_BSM_S_PRE_FULLCOM
                   * or CANSM_BSM_S_PRE_NOCOM choose the decision point between
                   * Partial network / no partial network and don't execute effect
                   * E_PRE_NO_COM again (see SWS Figure 7-2). */
                  nextTransition = CANSM_T_RNOCO_DEINIT_INITIAL;
                  break;
              } /* switch */
            }
            else
            {
              CanSM_MainFunctionRepeatModeRequests(nw, internalState, &nextTransition);

              if (nextTransition == CANSM_T_NONE)
              {
                /* after mode request repetition, decrement counter, restart timer  */
                CANSM_NW_INFO_MODE_REQUEST_REPETITION_COUNTER(nw)--;
                CANSM_NW_INFO_MODE_REQUEST_REPETITION_TIMER(nw) = CANSM_MODEREQ_REPEAT_TIME;
              }
            }
          }
        }
      }
      else
      {
        /* do nothing */
      }
    } /* nextTransition == CANSM_T_NONE */

    /* If the MainFunction shall make a transition do them now */
    if (nextTransition != CANSM_T_NONE)
    {
      CanSM_DoTransitionSequence(nw, nextTransition, TRUE);
    }

    /* b3.) synchronous part of transition done, unlock protection */
    TS_AtomicAssign8(CANSM_NW_INFO_TRANSITION_ONGOING(nw), FALSE);

  } /* DoMainFunctionProcessing == TRUE */
}

FUNC(void, CANSM_CODE) CanSM_DoTransitionSequence(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType initialTransition,
  const boolean invokedInMainFunction
)
{
  /*
   * Overview
   *
   * do
   * {
   *   switch (transition)
   *   {
   *     case some transition:
   *       execute transition;
   *       if (...)
   *       {
   *         set next transition;
   *       }
   *       if (...)
   *       {
   *         execute critical section;
   *       }
   *       break;
   *   }
   * } while (next transition set);
   *
   */

  CanSM_TransitionType nextTransition = initialTransition;

  /* - when changing state, reset mode request repetition counter/timer before trailing
   *   critical section
   * - if not in main function context, increment timer value by 1, since next main function
   *   invocation might happen immediately after this transition
   */
  CANSM_NW_INFO_MODE_REQUEST_REPETITION_TIMER(nw) = invokedInMainFunction ?
    (uint16)CANSM_MODEREQ_REPEAT_TIME : ((uint16)CANSM_MODEREQ_REPEAT_TIME + 1U);
  CANSM_NW_INFO_MODE_REQUEST_REPETITION_COUNTER(nw) = CANSM_MODEREQ_MAX;


  /* loop until requested mode is reached or mode indication is pending */
  /* (synchronous mode indications trigger next transition) */
  do /* ... while (nextTransition != CANSM_T_NONE) */
  {
    const CanSM_TransitionType transition = nextTransition;

    nextTransition = CANSM_T_NONE;

    CanSM_DoTransitionSwitch(
      nw,
      transition,
      &nextTransition,
      invokedInMainFunction
    );

  } while (nextTransition != CANSM_T_NONE);
}

FUNC(void, CANSM_CODE) CanSM_VerifyRequestedComMode
(
  const ComM_ModeType reqMode,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* default value for next transition */
  *nextTransition = CANSM_T_NONE;

  switch (reqMode)
  {
    /* we are in mode xxx --> we have a request for mode NO_COMMUNICATION */
    case COMM_NO_COMMUNICATION:
      if (
           (internalState == CANSM_S_FUCO_NO_BUS_OFF)    /* as specified in SWS */
           ||
           (internalState == CANSM_S_FUCO_BUS_OFF_CHECK) /* in addition to SWS transition */
           ||
           (internalState == CANSM_S_FUCO_TX_OFF)        /* in addition to SWS transition */
         )
      {
        /* to reach NO_COMMUNICATION we have to make a transition
         * via SILENT_COMMUNICATION (refer to Figure 7-1 in SWS)
         */
        *nextTransition = CANSM_T_FUCO_SICO;
      }
      else if (internalState == CANSM_S_SICO)
      {
        *nextTransition = CANSM_T_RNOCO_INITIAL;
      }
      else
      {
        /* nothing to do */
      }
      break;
    /* we are in mode xxx --> we have a request for mode SILENT_COMMUNICATION */
    case COMM_SILENT_COMMUNICATION:
      if ( (internalState == CANSM_S_FUCO_NO_BUS_OFF)    /* as specified in SWS */
           ||
           (internalState == CANSM_S_FUCO_BUS_OFF_CHECK) /* in addition to SWS transition */
           ||
           (internalState == CANSM_S_FUCO_TX_OFF)        /* in addition to SWS transition */
         )
      {
        *nextTransition = CANSM_T_FUCO_SICO;
      }
      else if (internalState == CANSM_S_NOCO)
      {
        /* We allow a request to SILENT_COMMUNICATION although
           forbidden in the SWS (see CANSM402).
           By doing this via state FULL_COMMUNICATION we only
           have SWS conform mode transitions. */
        *nextTransition = CANSM_T_RFUCO_INITIAL;
      }
      else
      {
        /* nothing to do */
      }
      break;
    /* we are in mode xxx --> we have a request for mode FULL_COMMUNICATION */
    case COMM_FULL_COMMUNICATION:
      if (internalState == CANSM_S_NOCO)
      {
        *nextTransition = CANSM_T_RFUCO_INITIAL;
      }
      else if (internalState == CANSM_S_SICO)
      {
        *nextTransition = CANSM_T_SICO_FUCO;
      }
      else
      {
        /* nothing to do */
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_REQUESTCOMMODE);
      break;
    /* CHECK: PARSE */
  }
}

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>



/* -----------------[ Transitions ]---------------------------------------- */

STATIC FUNC(void, CANSM_CODE) CanSM_DoTransitionSwitch(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition,
  const boolean invokedInMainFunction
)
{
  const CanSM_TransitionType mainTransition = CANSM_GET_MAIN_TRANSITION(transition);

  switch (mainTransition)
  {
    /* handle transitions assigned to main state "Silent Communication" */
    case CANSM_T_SICO_FUCO:
      CanSM_Do_T_SICO_FUCO(nw, invokedInMainFunction);
      break;
    /* handle transitions assigned to main state "Full Communication" */
    case CANSM_T_FUCO:
      CanSM_Do_T_FUCO(nw, transition, nextTransition);
      break;
    /* handle transitions assigned to main state "Request No Communication" */
    case CANSM_T_RNOCO:
      CanSM_Do_T_RNOCO(nw, transition, nextTransition);
      break;
    /* handle transitions assigned to main state "Request Full Communication" */
    case CANSM_T_RFUCO:
      CanSM_Do_T_RFUCO(nw, transition, nextTransition, invokedInMainFunction);
      break;
#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
    /* handle transitions assigned to main state "Partial Network" */
    case CANSM_T_PN:
      CanSM_Do_T_PN(nw, transition, nextTransition);
      break;
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */
#if (CANSM_SET_BAUDRATE_API == STD_ON)
    /* handle transitions assigned to main state "Change Baudrate" */
    case CANSM_T_CHBR:
      CanSM_Do_T_CHBR(nw, transition, nextTransition);
      break;
#endif /* CANSM_SET_BAUDRATE_API */
#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
    /* handle transitions assigned to main state "Tx Timeout Exception" */
    case CANSM_T_TX_TIMEOUT:
      CanSM_Do_T_TX_TIMEOUT(nw, transition, nextTransition);
      break;
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (transition)
  {
    /* go to silent communication */
    case CANSM_T_FUCO_SICO:
      CanSM_Do_T_FUCO_SICO(nw);
      break;

    /* bus-off recovery state machine */
    case CANSM_T_FUCO_HANDLE_BUS_OFF:
      CanSM_Do_T_FUCO_HANDLE_BUS_OFF(nw, nextTransition);
      break;
    case CANSM_T_FUCO_TX_OFF:
      CanSM_Do_T_FUCO_TX_OFF(nw);
      break;
#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
    case CANSM_T_FUCO_OFF:
      CanSM_Do_T_FUCO_OFF(nw, nextTransition);
      break;
    case CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY:
      CANSM_Do_T_FUCO_BUS_DEACTIVATED_RECOVERY(nw, nextTransition);
      break;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */
    case CANSM_T_FUCO_TX_ON:
      CanSM_Do_T_FUCO_TX_ON(nw);
      break;
    case CANSM_T_FUCO_BUS_OFF_PASSIVE:
      CanSM_Do_TRY_T_FUCO_BUS_OFF_PASSIVE(nw);
      break;

#if (CANSM_SET_BAUDRATE_API == STD_ON)
    /* change baudrate state machine */
    case CANSM_T_FUCO_CHBR:
      CanSM_Do_T_FUCO_CHBR(nw, nextTransition);
      break;
#endif

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (transition)
  {

    case CANSM_T_RNOCO_INITIAL:
      CanSM_Do_T_RNOCO_INITIAL(nw, nextTransition);
      break;
    case CANSM_T_RNOCO_DEINIT_INITIAL:
      CanSM_Do_T_RNOCO_DEINIT_INITIAL(nw, nextTransition);
      break;
    case CANSM_T_RNOCO_CC_INITIAL:
      CanSM_Do_T_RNOCO_CC_INITIAL(nw, nextTransition);
      break;
    case CANSM_T_RNOCO_CC_STOPPED:
      CanSM_Do_T_RNOCO_CC_STOPPED(nw, nextTransition);
      break;
    case CANSM_T_RNOCO_CC_SLEEP:
      CanSM_Do_T_RNOCO_CC_SLEEP(nw, nextTransition);
      break;

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_T_RNOCO_TRCV_NORMAL:
      CanSM_Do_T_RNOCO_TRCV_NORMAL(nw, nextTransition);
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_T_RNOCO_FINAL:
      CanSM_Do_T_RNOCO_FINAL(nw);
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition,
  const boolean invokedInMainFunction
)
{

  switch (transition)
  {

    case CANSM_T_RFUCO_INITIAL:
      CanSM_Do_T_RFUCO_INITIAL(nw, nextTransition);
      break;

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_T_RFUCO_TRCV_INITIAL:
      CanSM_Do_T_RFUCO_TRCV_INITIAL(nw, nextTransition);
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_T_RFUCO_TRCV_NORMAL:
      CanSM_Do_T_RFUCO_TRCV_NORMAL(nw, nextTransition);
      break;
    case CANSM_T_RFUCO_CC_STOPPED:
      CanSM_Do_T_RFUCO_CC_STOPPED(nw, nextTransition);
      break;
    case CANSM_T_RFUCO_CC_STARTED:
      CanSM_Do_T_RFUCO_CC_STARTED(nw, invokedInMainFunction);
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (transition)
  {

    case CANSM_T_PN_DEINIT:
      CanSM_Do_T_PN_DEINIT(nw,nextTransition);
      break;
    case CANSM_T_PN_CLEAR_WUF_INDICATED:
      CanSM_Do_T_PN_CLEAR_WUF_INDICATED(nw,nextTransition);
      break;
    case CANSM_T_PN_CC_STOPPED_INDICATED:
      CanSM_Do_T_PN_CC_STOPPED_INDICATED(nw,nextTransition);
      break;
    case CANSM_T_PN_TRCV_NORMAL_INDICATED:
      CanSM_Do_T_PN_TRCV_NORMAL_INDICATED(nw,nextTransition);
      break;
    case CANSM_T_PN_TRCV_STANDBY_INDICATED:
      CanSM_Do_T_PN_TRCV_STANDBY_INDICATED(nw,nextTransition);
      break;
    case CANSM_T_PN_CC_SLEEP_INDICATED:
      CanSM_Do_T_PN_CC_SLEEP_INDICATED(nw,nextTransition);
      break;
    case CANSM_T_PN_CC_SLEEP_TIMEOUT:
      CanSM_Do_T_PN_CC_SLEEP_TIMEOUT(nw,nextTransition);
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

#if (CANSM_SET_BAUDRATE_API == STD_ON)
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (transition)
  {

    case CANSM_T_CHBR_CC_STOP:
      CanSM_Do_T_CHBR_CC_STOP(nw, nextTransition);
      break;
    case CANSM_T_CHBR_STOPPED_INDICATED:
      CanSM_Do_T_CHBR_STOPPED_INDICATED(nw, nextTransition);
      break;
    case CANSM_T_CHBR_CC_START:
      CanSM_Do_T_CHBR_CC_START(nw, nextTransition);
      break;
    case CANSM_T_CHBR_STARTED_INDICATED:
      CanSM_Do_T_CHBR_STARTED_INDICATED(nw, nextTransition);
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}
#endif /* CANSM_SET_BAUDRATE_API */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT
(
  const CanSM_NetworkIndexType nw,
  const CanSM_TransitionType transition,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (transition)
  {

    case CANSM_T_TX_TIMEOUT_INITIAL:
      CanSM_Do_T_TX_TIMEOUT_INITIAL(nw, nextTransition);
      break;
    case CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED:
      CanSM_Do_T_TX_TIMEOUT_CC_STOPPED_INDICATED(nw, nextTransition);
      break;
    case CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED:
      CanSM_Do_T_TX_TIMEOUT_CC_STARTED_INDICATED(nw);
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */

  } /* end transition switch */
}
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */


/* transitions to NOCO */

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* Effect E_PRE_NO_COM (and E_PRE_NOCOM) of figure 7-1 in CanSM SWS. */
  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_NO_COMMUNICATION);

  /* default value set to state machine deinitialization without partial network. */
  *nextTransition = CANSM_T_RNOCO_DEINIT_INITIAL;
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_DEINIT_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* default value set to state machine deinitialization without partial network. */
  *nextTransition = CANSM_T_RNOCO_CC_INITIAL;

  /* only select partial network deinitialization if
   * a) partial network support enabled global AND
   * b) partial network enabled for this specific network AND
   * c) transceiver enabled global AND
   * d) transceiver enabled for this specific network */
#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_TRANSCEIVERS_NUM_MAX > 0) ) /* a, c */
  if (CANSM_NW_CONFIG_TRCV_ID(nw) != CANSM_TRANSCEIVER_ID_NONE) /* d */
  {
    /* Deviation MISRAC2012-2 */
    if( CANSM_PARTIALNETWORKING(nw) == TRUE ) /* b */
    {
      /* Deviation MISRAC2012-1 */
      *nextTransition = CANSM_T_PN_DEINIT;
    }
  }
#endif /* (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) */

#if (    (CANSM_PNSUPPORT == STD_OFF)       \
      || (CANSM_TRANSCEIVERS_NUM_MAX == 0) \
    )
  TS_PARAM_UNUSED(nw);
#endif
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STOPPED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RNOCO_CC_STOPPED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RNOCO_CAN_CC_STOPPED);
  }
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_STOPPED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_SLEEP,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RNOCO_CC_SLEEP;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RNOCO_CAN_CC_SLEEP);
  }
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_CC_SLEEP(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

#if (CANSM_TRANSCEIVERS_NUM_MAX > 0U)
  if (CANSM_NW_CONFIG_TRCV_ID(nw) != CANSM_TRANSCEIVER_ID_NONE)
  {
    boolean allModeIndicationsReceived = FALSE;

    CanSM_ClearTrcvModeIndAndSetMode(nw,
                                     CANTRCV_TRCVMODE_NORMAL,
                                     &allModeIndicationsReceived);
    if (allModeIndicationsReceived == TRUE)
    {
      *nextTransition = CANSM_T_RNOCO_TRCV_NORMAL;
    }
    else
    {
      CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RNOCO_CAN_TRCV_NORMAL);
    }
  }
  else
  {
    *nextTransition = CANSM_T_RNOCO_FINAL;
  }
#else /* CANSM_TRANSCEIVERS_NUM_MAX > 0U */

  *nextTransition = CANSM_T_RNOCO_FINAL;

  TS_PARAM_UNUSED(nw);
#endif /* CANSM_TRANSCEIVERS_NUM_MAX > 0U */
}


#if (CANSM_USE_TRANSCEIVER == STD_ON)

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_TRCV_NORMAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearTrcvModeIndAndSetMode(nw,
                                   CANTRCV_TRCVMODE_STANDBY,
                                   &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RNOCO_FINAL;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RNOCO_CAN_TRCV_STANDBY);
  }
}

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RNOCO_FINAL(
  const CanSM_NetworkIndexType nw
)
{
  /* in the initial transition to no communication, don't notify ComM, which may not be
   * initialized */
  boolean isInitialTransition = FALSE;

  if (CANSM_NW_INFO_STATE(nw) == CANSM_UNINITED)
  {
    isInitialTransition = TRUE;
  }

  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_NO_COMMUNICATION;
    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);

    /* change to NOCO before notifying other modules */
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_NOCO;
  }
  CANSM_EXIT_CRITICAL_SECTION();

  if (isInitialTransition == FALSE)
  {
    /* Effect E_NOCOM in figure 7-1 */
    CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommNoCommunication);
  }
}


/* transitions to NOCO with partial networking */

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_DEINIT(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean clearIndicationReceived = FALSE;

  /* reset indication flag */
  TS_AtomicAssign8(CANSM_NW_INFO_TRCVCLEARWUFINDICATION(nw), FALSE);

  CanSM_ClearTrcvWufFlag(nw, &clearIndicationReceived);

  if (clearIndicationReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_CLEAR_WUF_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_CLEAR_WUF);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CLEAR_WUF_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STOPPED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_CC_STOPPED_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_CC_STOPPED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearTrcvModeIndAndSetMode(nw,
                                   CANTRCV_TRCVMODE_NORMAL,
                                   &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_TRCV_NORMAL_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_TRCV_NORMAL);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_TRCV_NORMAL_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearTrcvModeIndAndSetMode(nw,
                                   CANTRCV_TRCVMODE_STANDBY,
                                   &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_TRCV_STANDBY_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_TRCV_STANDBY);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_TRCV_STANDBY_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_SLEEP,
                                    &allModeIndicationsReceived);

  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_CC_SLEEP_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_CC_SLEEP);
  }
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_SLEEP_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean checkIndicationReceived = FALSE;

  /* reset indication flag */
  TS_AtomicAssign8(CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw), FALSE);

  CanSM_CheckTrcvWakeFlag(nw, &checkIndicationReceived);

  if (checkIndicationReceived == TRUE)
  {
    *nextTransition = CANSM_T_RNOCO_FINAL;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_PN_CC_SLEEP_TIMEOUT(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean checkIndicationReceived = FALSE;

  /* reset indication flag */
  TS_AtomicAssign8(CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw), FALSE);

  CanSM_CheckTrcvWakeFlag(nw, &checkIndicationReceived);

  if (checkIndicationReceived == TRUE)
  {
    *nextTransition = CANSM_T_PN_DEINIT;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP);
  }
}

#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */


/* transitions to FUCO */

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

#if (CANSM_TRANSCEIVERS_NUM_MAX > 0U)
  if (CANSM_NW_CONFIG_TRCV_ID(nw) != CANSM_TRANSCEIVER_ID_NONE)
  {
    *nextTransition = CANSM_T_RFUCO_TRCV_INITIAL;
  }
  else
#endif /* CANSM_TRANSCEIVERS_NUM_MAX > 0U */
  {
    *nextTransition = CANSM_T_RFUCO_TRCV_NORMAL;
  }

#if (CANSM_TRANSCEIVERS_NUM_MAX == 0U)
  TS_PARAM_UNUSED(nw);
#endif /* (CANSM_TRANSCEIVERS_NUM_MAX == 0U) */
}


#if (CANSM_USE_TRANSCEIVER == STD_ON)

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_TRCV_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearTrcvModeIndAndSetMode(nw,
                                   CANTRCV_TRCVMODE_NORMAL,
                                   &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RFUCO_TRCV_NORMAL;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RFUCO_CAN_TRCV_NORMAL);
  }
}

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_TRCV_NORMAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STOPPED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RFUCO_CC_STOPPED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RFUCO_CAN_CC_STOPPED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_CC_STOPPED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  {
    /* reset bus-off flag before starting controllers */
    uint8 buf = CANSM_NO_BUS_OFF;
    TS_AtomicAssign8(CANSM_NW_INFO_BUSOFF(nw), buf);
  }

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STARTED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_RFUCO_CC_STARTED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_RFUCO_CAN_CC_STARTED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_RFUCO_CC_STARTED(
  const CanSM_NetworkIndexType nw,
  const boolean invokedInMainFunction
)
{
  /* reset bus-off counter before entering full communication */
  CANSM_NW_INFO_BOR_COUNTER(nw) = 0U;

  CanSM_SetPduModes(nw, CANIF_ONLINE);

  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_FULL_COMMUNICATION);

  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_FULL_COMMUNICATION;
    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);

    /* change to FUCO before notifying other modules */
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_BUS_OFF_CHECK;

#if (CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON)
    /* Deviation MISRAC2012-2 */
    if (CANSM_BOR_TX_CONFIRMATION_POLLING(nw) == FALSE)
    {
      CANSM_NW_INFO_TIMER(nw) = invokedInMainFunction ?
        (uint16)CANSM_BOR_TIME_TX_ENSURED(nw) : ((uint16)CANSM_BOR_TIME_TX_ENSURED(nw) + 1U);
    }
#else /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
    TS_PARAM_UNUSED(invokedInMainFunction);
#endif /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
  }
  CANSM_EXIT_CRITICAL_SECTION();

  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommFullCommunication);
}

/* transitions between FUCO and SICO */

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_SICO(
  const CanSM_NetworkIndexType nw
)
{
  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_SILENT_COMMUNICATION);

  CanSM_SetPduModes(nw, CANIF_TX_OFFLINE);

  /* internal state was already updated in CanSM_RequestComModeInternal() to prevent race
   * condition */

  TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), CANSM_SILENT_COMMUNICATION);

  CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_SICO;

  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommSilentCommunication);
}


STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_SICO_FUCO(
  const CanSM_NetworkIndexType nw,
  const boolean invokedInMainFunction
)
{
  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_FULL_COMMUNICATION);

  CanSM_SetPduModes(nw, CANIF_ONLINE);

  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_FULL_COMMUNICATION;
    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);

    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_BUS_OFF_CHECK;

#if (CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON)
    /* Deviation MISRAC2012-2 */
    if (CANSM_BOR_TX_CONFIRMATION_POLLING(nw) == FALSE)
    {
      /* wait long enough if invoked outside of main function context */
      CANSM_NW_INFO_TIMER(nw) = invokedInMainFunction ?
        (uint16)CANSM_BOR_TIME_TX_ENSURED(nw) : ((uint16)CANSM_BOR_TIME_TX_ENSURED(nw) + 1U);
    }
#else /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
    TS_PARAM_UNUSED(nw);
    TS_PARAM_UNUSED(invokedInMainFunction);
#endif /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
  }
  CANSM_EXIT_CRITICAL_SECTION();

  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommFullCommunication);
}

/* Transitions for Baudrate API */

#if (CANSM_SET_BAUDRATE_API == STD_ON)

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_CHBR(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  TS_PARAM_UNUSED(nw);

  /* E_CHANGE_BR_BSWM_MODE*/
  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_CHANGE_BAUDRATE);

  /* immediatelys trigger next transition */
  *nextTransition = CANSM_T_CHBR_CC_STOP;
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_CC_STOP(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  /* DO_SET_CC_MODE_STOPPED */
  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STOPPED,
                                    &allModeIndicationsReceived);

  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_CHBR_STOPPED_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_CHBR_CAN_CC_STOPPED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  uint8_least i;

  TS_PARAM_UNUSED (nw);

  /* E_CHANGE_BAUDRATE */
  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommNoCommunication);

  for ( i=0U; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    (void)CanIf_SetBaudrate(CANSM_NW_CONFIG_CTRL(nw, i), CANSM_NW_INFO_BAUDRATEID(nw));
  }

  /* immediatelys trigger next transition */
  *nextTransition = CANSM_T_CHBR_CC_START;
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_CC_START(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  /* DO_SET_CC_MODE_STOPPED */
  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STARTED,
                                    &allModeIndicationsReceived);

  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_CHBR_STARTED_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_CHBR_CAN_CC_STARTED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_CHBR_STARTED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
    TS_PARAM_UNUSED(nw);
    switch(CANSM_NW_INFO_REQCOMMODE(nw))
    {
      case COMM_FULL_COMMUNICATION:
        *nextTransition = CANSM_T_RFUCO_CC_STARTED;
        break;
      case COMM_SILENT_COMMUNICATION:
        *nextTransition = CANSM_T_FUCO_SICO;
        break;
      case COMM_NO_COMMUNICATION:
      default:
        *nextTransition = CANSM_T_RNOCO_INITIAL;
        break;
    }
}
#endif /* CANSM_SET_BAUDRATE_API */

/* bus-off recovery state machine */

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_HANDLE_BUS_OFF(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF)
  boolean allModeIndicationsReceived = FALSE;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF */

  boolean BusOffLevel2Reached;
  CanSM_BswMCurrentStateType BswMState;

  BusOffLevel2Reached = (CANSM_NW_INFO_BOR_COUNTER(nw) < CANSM_BOR_COUNTER_L1_TO_L2(nw))
    ? FALSE : TRUE;

  if (CANSM_NW_INFO_BOR_COUNTER(nw) != 0xFFU)
  {
    CANSM_NW_INFO_BOR_COUNTER(nw)++;
  }

  /* report BswM state for bus-off dependent on configuration parameter
     CanSMEnhancedBusOffReporting*/
#if (CANSM_ENHANCED_BUSOFF_REPORTING == STD_ON)
  BswMState = (BusOffLevel2Reached == FALSE) ?
      (CanSM_BswMCurrentStateType)CANSM_BSWM_BUS_OFF_L1 :
      (CanSM_BswMCurrentStateType)CANSM_BSWM_BUS_OFF_L2;
#else /* CANSM_ENHANCED_BUSOFF_REPORTING == STD_ON */
  BswMState = CANSM_BSWM_BUS_OFF;
#endif /* CANSM_ENHANCED_BUSOFF_REPORTING == STD_ON */

  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), BswMState);

#if (CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM)
    /* Deviation MISRAC2012-2 */
    CANSM_REPORT_DEM_BUS_OFF_FAILED(nw);
#elif (CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DET)
    CANSM_DET_WITH_INSTANCEID(
        CANSM_NW_HANDLE_FROM_IDX(nw),
        CANSM_SID_MAINFUNCTION,
        CANSM_E_DEMTODET_BUS_OFF
    );
#endif /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */

  /* contrary to SWS, set PDU mode now to avoid transmission after controller restart */
#if(CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF)
  CanSM_SetPduModes(nw, CANIF_TX_OFFLINE);
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF */

  /* middle critical section */
  CANSM_ENTER_CRITICAL_SECTION();
  {
    /* make bus-off recovery visible to CanSM_RequestComMode by updating internal state; reset
     * bus-off flag */
    uint8 buf = CANSM_NO_BUS_OFF;
    TS_AtomicAssign8(CANSM_NW_INFO_BUSOFF(nw), buf);

#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_OFF;
#else /* CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF */
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_RESTART_CC;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */


    if (BusOffLevel2Reached == FALSE)
    {
      CANSM_NW_INFO_TIMER(nw) = CANSM_BOR_TIME_L1(nw);
    }
    else
    {
      CANSM_NW_INFO_TIMER(nw) = CANSM_BOR_TIME_L2(nw);
    }
  }
  CANSM_EXIT_CRITICAL_SECTION();

  /* For bus-off recovery, don't reset mode indications here!
   * This handling in necessary to make bus-off recovery for a network with
   * multiple underlying CanSMController.
   * The callback CanSM_ControllerBusOff sets the state to STOPPED for each
   * controller signalizing a bus-off.
   * All other controllers of this network are still in state STARTED.
   * It is necessary to restart only the controllers which signlized the bus-off
   * to avoid invalid transitions in the CAN driver. */
  /* !LINKSTO CanSM.EB.BusOffRecovery.RestartCtrl,1 */

#if(CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF)

  CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_FUCO_TX_OFF;
  }
#else  /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */
    *nextTransition = CANSM_T_FUCO_OFF;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF */
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_TX_OFF(
  const CanSM_NetworkIndexType nw
)
{
  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_SILENT_COMMUNICATION;

    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);

    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_TX_OFF;
  }
  CANSM_EXIT_CRITICAL_SECTION();

  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommSilentCommunication);

}

#if (CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_OFF(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  CanSM_SetPduModes(nw, CANIF_OFFLINE);
  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_NO_COMMUNICATION;
    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);
    /* perform T_FUCO_BUS_OFF_PASSIVE inside critical section */
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY;
  }
  CANSM_EXIT_CRITICAL_SECTION();

  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommNoCommunication);

  *nextTransition = CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY;
}


STATIC FUNC(void, CANSM_CODE) CANSM_Do_T_FUCO_BUS_DEACTIVATED_RECOVERY(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  if(CANSM_NW_INFO_TIMER(nw) == 0U)
  {
    boolean allModeIndicationsReceived = FALSE;

    CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);

    /* check if indication was received */
    if (allModeIndicationsReceived == TRUE)
    {
      *nextTransition = CANSM_T_FUCO_TX_ON;
    }
    else
    {
      CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_RESTART_CC;
    }
  }
}
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_FUCO_TX_ON(
  const CanSM_NetworkIndexType nw
)
{
  CanSM_SetPduModes(nw, CANIF_ONLINE);

  CANSM_ENTER_CRITICAL_SECTION();
  {
    uint8 buf = CANSM_FULL_COMMUNICATION;
    TS_AtomicAssign8(CANSM_NW_INFO_STATE(nw), buf);

    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_BUS_OFF_CHECK;

#if (CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON)
    /* Deviation MISRAC2012-2 */
    if (CANSM_BOR_TX_CONFIRMATION_POLLING(nw) == FALSE)
    {
      CANSM_NW_INFO_TIMER(nw) = CANSM_BOR_TIME_TX_ENSURED(nw);
    }
#endif /* CANSM_USE_BOR_TIME_TX_ENSURED == STD_ON */
  }
  CANSM_EXIT_CRITICAL_SECTION();

  BSWM_CANSM_CURRENTSTATE(CANSM_NW_HANDLE_FROM_IDX(nw), CANSM_BSWM_FULL_COMMUNICATION);
  CANSM_CALL_SM_MODEINDICATION(nw, CanSM_CommFullCommunication);
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_TRY_T_FUCO_BUS_OFF_PASSIVE(
  const CanSM_NetworkIndexType nw
)
{
  CANSM_ENTER_CRITICAL_SECTION();
  {
    /* perform T_FUCO_BUS_OFF_PASSIVE inside critical section */
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_NO_BUS_OFF;
    CANSM_NW_INFO_BOR_COUNTER(nw) = 0U;
  }
  CANSM_EXIT_CRITICAL_SECTION();

#if (CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM)
  /* Deviation MISRAC2012-2 */
  CANSM_REPORT_DEM_BUS_OFF_PASSED(nw);
#endif /* CANSM_PROD_ERR_HANDLING_BUS_OFF == TS_PROD_ERR_REP_TO_DEM */
}

/* Transitions for TxTimeout exception handling */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_INITIAL(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STOPPED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_TX_TIMEOUT_CC_STOPPED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_CC_STOPPED_INDICATED(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  boolean allModeIndicationsReceived = FALSE;

  CanSM_ClearCtrlModeIndAndSetModes(nw,
                                    CANIF_CS_STARTED,
                                    &allModeIndicationsReceived);
  if (allModeIndicationsReceived == TRUE)
  {
    *nextTransition = CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED;
  }
  else
  {
    CanSM_DefaultTrailingCriticalSection(nw, CANSM_S_TX_TIMEOUT_CC_STARTED);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_Do_T_TX_TIMEOUT_CC_STARTED_INDICATED(
  const CanSM_NetworkIndexType nw
)
{
  /* CanSM.ASR43.SWS_CANSM_00655 */
  CanSM_SetPduModes(nw, CANIF_ONLINE);

  CANSM_ENTER_CRITICAL_SECTION();
  {
    CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_FUCO_NO_BUS_OFF;
  }
  CANSM_EXIT_CRITICAL_SECTION();
}
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */


/* CanSM_MainFunctionInternal subroutines */

STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  const CanSM_InternalStateType MainInternalState = CANSM_GET_MAIN_STATE(internalState);

  switch (MainInternalState)
  {

    /* states when requesting No Communication */
    case CANSM_S_RNOCO:
      CanSM_MainFunctionCheckModeIndications_S_RNOCO(nw, internalState, nextTransition);
      break;
    /* states when requesting Full Communication */
    case CANSM_S_RFUCO:
      CanSM_MainFunctionCheckModeIndications_S_RFUCO(nw, internalState, nextTransition);
      break;
#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
    /* states when requesting Partial Network */
    case CANSM_S_PN:
      CanSM_MainFunctionCheckModeIndications_S_PN(nw, internalState, nextTransition);
      break;
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */
    default:
    {
      /* all other states */
      switch (internalState)
      {
        /* BOR */
        case CANSM_S_FUCO_RESTART_CC:
          if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STARTED) == TRUE)
          {
#if(CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
            *nextTransition = CANSM_T_FUCO_TX_ON;
#else /* CANSM_BUSDEACTIVATED_BUSOFF == STD_OFF */
            *nextTransition = CANSM_T_FUCO_TX_OFF;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */
          }
          break;
#if (CANSM_SET_BAUDRATE_API == STD_ON)
        /* Baudrate */
        case CANSM_S_CHBR_CAN_CC_STOPPED:
          if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STOPPED) == TRUE)
          {
            *nextTransition = CANSM_T_CHBR_STOPPED_INDICATED;
          }
          break;

        case CANSM_S_CHBR_CAN_CC_STARTED:
          if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STARTED) == TRUE)
          {
            *nextTransition = CANSM_T_CHBR_STARTED_INDICATED;
          }
          break;
#endif /* CANSM_SET_BAUDRATE_API */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
        /* TxTimeout Exception */
        case CANSM_S_TX_TIMEOUT_CC_STOPPED:
          if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STOPPED) == TRUE)
          {
            *nextTransition = CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED;
          }
          break;
        case CANSM_S_TX_TIMEOUT_CC_STARTED:
          if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STARTED) == TRUE)
          {
            *nextTransition = CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED;
          }
          break;
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

        /* CHECK: NOPARSE */
        default:
          CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
          break;
        /* CHECK: PARSE */
      }
    }
    break;
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (internalState)
  {

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RNOCO_CAN_TRCV_NORMAL:
      if (CanSM_CheckTrcvModeIndication(nw, CANTRCV_TRCVMODE_NORMAL) == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_TRCV_NORMAL;
      }
      break;

    case CANSM_S_RNOCO_CAN_TRCV_STANDBY:
      if (CanSM_CheckTrcvModeIndication(nw, CANTRCV_TRCVMODE_STANDBY) == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_FINAL;
      }
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_S_RNOCO_CAN_CC_STOPPED:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STOPPED) == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_CC_STOPPED;
      }
      break;

    case CANSM_S_RNOCO_CAN_CC_SLEEP:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_SLEEP) == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_CC_SLEEP;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (internalState)
  {

    case CANSM_S_PN_CLEAR_WUF:
      if (CANSM_NW_INFO_TRCVCLEARWUFINDICATION(nw) == TRUE)
      {
        *nextTransition = CANSM_T_PN_CLEAR_WUF_INDICATED;
      }
      break;

    case CANSM_S_PN_CC_STOPPED:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STOPPED) == TRUE)
      {
        *nextTransition = CANSM_T_PN_CC_STOPPED_INDICATED;
      }
      break;

    case CANSM_S_PN_TRCV_NORMAL:
      if (CanSM_CheckTrcvModeIndication(nw, CANTRCV_TRCVMODE_NORMAL) == TRUE)
      {
        *nextTransition = CANSM_T_PN_TRCV_NORMAL_INDICATED;
      }
      break;

    case CANSM_S_PN_TRCV_STANDBY:
      if (CanSM_CheckTrcvModeIndication(nw, CANTRCV_TRCVMODE_STANDBY) == TRUE)
      {
        *nextTransition = CANSM_T_PN_TRCV_STANDBY_INDICATED;
      }
      break;

    case CANSM_S_PN_CC_SLEEP:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_SLEEP) == TRUE)
      {
        *nextTransition = CANSM_T_PN_CC_SLEEP_INDICATED;
      }
      break;

    case CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP:
      if (CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw) == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_FINAL;
      }
      break;

    case CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP:
      if (CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw) == TRUE)
      {
        *nextTransition = CANSM_T_PN_DEINIT;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionCheckModeIndications_S_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{

  switch (internalState)
  {

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RFUCO_CAN_TRCV_NORMAL:
      if (CanSM_CheckTrcvModeIndication(nw, CANTRCV_TRCVMODE_NORMAL) == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_TRCV_NORMAL;
      }
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_S_RFUCO_CAN_CC_STOPPED:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STOPPED) == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_CC_STOPPED;
      }
      break;

    case CANSM_S_RFUCO_CAN_CC_STARTED:
      if (CanSM_CheckCtrlModeIndications(nw, CANIF_CS_STARTED) == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_CC_STARTED;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* set by helper functions which request ctrl/trcv mode, initialized to prevent compiler
   * warning */
  boolean allModeIndicationsReceived = FALSE;
  const CanSM_InternalStateType mainInternalState = CANSM_GET_MAIN_STATE(internalState);

  switch (mainInternalState)
  {

    /* states when requesting No Communication */
    case CANSM_S_RNOCO:
      CanSM_MainFunctionRepeatModeRequests_S_RNOCO(nw, internalState, nextTransition);
      break;

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
    /* states when requesting Partial Network */
    case CANSM_S_PN:
      CanSM_MainFunctionRepeatModeRequests_S_PN(nw, internalState, nextTransition);
      break;
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

    /* states when requesting Full Communication */
    case CANSM_S_RFUCO:
      CanSM_MainFunctionRepeatModeRequests_S_RFUCO(nw, internalState, nextTransition);
      break;

    default:
    {
      switch (internalState)
      {

        /* BOR */
        case CANSM_S_FUCO_RESTART_CC:
          CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);

          if (allModeIndicationsReceived == TRUE)
          {
#if(CANSM_BUSDEACTIVATED_BUSOFF == STD_ON)
            *nextTransition = CANSM_T_FUCO_TX_ON;
#else
            *nextTransition = CANSM_T_FUCO_TX_OFF;
#endif /* CANSM_BUSDEACTIVATED_BUSOFF == STD_ON */
          }
          break;
#if (CANSM_SET_BAUDRATE_API == STD_ON)
        /* Baudrate */
        case CANSM_S_CHBR_CAN_CC_STOPPED:
          CanSM_SetCtrlModes(nw, CANIF_CS_STOPPED, &allModeIndicationsReceived);

          if (allModeIndicationsReceived == TRUE)
          {
            *nextTransition = CANSM_T_CHBR_STOPPED_INDICATED;
          }
          break;

        case CANSM_S_CHBR_CAN_CC_STARTED:
          CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);

          if (allModeIndicationsReceived == TRUE)
          {
            *nextTransition = CANSM_T_CHBR_STARTED_INDICATED;
          }
          break;
#endif /* CANSM_SET_BAUDRATE_API */

#if (CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON)
        /* TxTimeout Exception */
        case CANSM_S_TX_TIMEOUT_CC_STOPPED:
          CanSM_SetCtrlModes(nw, CANIF_CS_STOPPED, &allModeIndicationsReceived);

          if (allModeIndicationsReceived == TRUE)
          {
            *nextTransition = CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED;
          }
          break;

        case CANSM_S_TX_TIMEOUT_CC_STARTED:
          CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);

          if (allModeIndicationsReceived == TRUE)
          {
            *nextTransition = CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED;
          }
          break;
#endif /* CANSM_TX_TIMEOUT_EXCEPTION_API == STD_ON */

        /* CHECK: NOPARSE */
        default:
          CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
          break;
        /* CHECK: PARSE */
      }
    }
    break;
  }
}


STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_RNOCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* set by helper functions which request ctrl/trcv mode, initialized to prevent compiler
   * warning */
  boolean allModeIndicationsReceived = FALSE;

  switch (internalState)
  {

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RNOCO_CAN_TRCV_NORMAL:
      CanSM_SetTrcvMode(nw, CANTRCV_TRCVMODE_NORMAL, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_TRCV_NORMAL;
      }
      break;

    case CANSM_S_RNOCO_CAN_TRCV_STANDBY:
      CanSM_SetTrcvMode(nw, CANTRCV_TRCVMODE_STANDBY, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_FINAL;
      }
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_S_RNOCO_CAN_CC_STOPPED:
      CanSM_SetCtrlModes(nw, CANIF_CS_STOPPED, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_CC_STOPPED;
      }
      break;

    case CANSM_S_RNOCO_CAN_CC_SLEEP:
      CanSM_SetCtrlModes(nw, CANIF_CS_SLEEP, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_CC_SLEEP;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )
STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_PN
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* set by helper functions which request ctrl/trcv mode, initialized to prevent compiler
   * warning */
  boolean allModeIndicationsReceived = FALSE;

  switch (internalState)
  {

    case CANSM_S_PN_CLEAR_WUF:
      CanSM_ClearTrcvWufFlag(nw, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_PN_CLEAR_WUF_INDICATED;
      }
      break;

    case CANSM_S_PN_CC_STOPPED:
      CanSM_SetCtrlModes(nw, CANIF_CS_STOPPED, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_PN_CC_STOPPED_INDICATED;
      }
      break;

    case CANSM_S_PN_TRCV_NORMAL:
      CanSM_SetTrcvMode(nw, CANTRCV_TRCVMODE_NORMAL, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_PN_TRCV_NORMAL_INDICATED;
      }
      break;

    case CANSM_S_PN_TRCV_STANDBY:
      CanSM_SetTrcvMode(nw, CANTRCV_TRCVMODE_STANDBY, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_PN_TRCV_STANDBY_INDICATED;
      }
      break;

    case CANSM_S_PN_CC_SLEEP:
      {
        *nextTransition = CANSM_T_PN_CC_SLEEP_TIMEOUT;
      }
      break;

    case CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP:
      CanSM_CheckTrcvWakeFlag(nw, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RNOCO_FINAL;
      }
      break;

    case CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP:
      CanSM_CheckTrcvWakeFlag(nw, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_PN_DEINIT;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}
#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

STATIC FUNC(void, CANSM_CODE) CanSM_MainFunctionRepeatModeRequests_S_RFUCO
(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType internalState,
  CONSTP2VAR(CanSM_TransitionType, AUTOMATIC, AUTOMATIC) nextTransition
)
{
  /* set by helper functions which request ctrl/trcv mode, initialized to prevent compiler
   * warning */
  boolean allModeIndicationsReceived = FALSE;

  switch (internalState)
  {

#if (CANSM_USE_TRANSCEIVER == STD_ON)
    case CANSM_S_RFUCO_CAN_TRCV_NORMAL:
      CanSM_SetTrcvMode(nw, CANTRCV_TRCVMODE_NORMAL, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_TRCV_NORMAL;
      }
      break;
#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

    case CANSM_S_RFUCO_CAN_CC_STOPPED:
      CanSM_SetCtrlModes(nw, CANIF_CS_STOPPED, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_CC_STOPPED;
      }
      break;

    case CANSM_S_RFUCO_CAN_CC_STARTED:
      CanSM_SetCtrlModes(nw, CANIF_CS_STARTED, &allModeIndicationsReceived);

      if (allModeIndicationsReceived == TRUE)
      {
        *nextTransition = CANSM_T_RFUCO_CC_STARTED;
      }
      break;

    /* CHECK: NOPARSE */
    default:
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_SID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }
}



/* default trailing critical section */

STATIC FUNC(void, CANSM_CODE) CanSM_DefaultTrailingCriticalSection(
  const CanSM_NetworkIndexType nw,
  const CanSM_InternalStateType newInternalState)
{
  CANSM_ENTER_CRITICAL_SECTION();
  CANSM_NW_INFO_INTERNALSTATE(nw) = newInternalState;
  CANSM_EXIT_CRITICAL_SECTION();
}



/* controller helper functions */

STATIC FUNC(void, CANSM_CODE) CanSM_SetPduModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_PduModeType pduModeRequest
)
{
  uint8_least i;

  for ( i=0U; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    (void) CanIf_SetPduMode(CANSM_NW_CONFIG_CTRL(nw, i), pduModeRequest);
  }
}

STATIC FUNC(void, CANSM_CODE) CanSM_ClearCtrlModeIndAndSetModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) allModeIndicationsReceived
)
{
  uint8_least i;

  for ( i=0U; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    uint8 buf = CANSM_CTRLMODETYPE_RESET;
    TS_AtomicAssign8(CanSM_CtrlModeIndication[CanSM_CtrlIdxFromCtrlId(CANSM_NW_CONFIG_CTRL(nw, i))], buf);
  }

  CanSM_SetCtrlModes(nw, ctrlMode, allModeIndicationsReceived);
}

STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckCtrlModeIndications(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode
)
{
  boolean result = TRUE;
  uint8_least i;

  for ( i=0U; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    uint8 buf;
    TS_AtomicAssign8(buf, CanSM_CtrlModeIndication[CanSM_CtrlIdxFromCtrlId(CANSM_NW_CONFIG_CTRL(nw, i))]);

    if (buf != CanSM_ControllerModeTypeToUint8(ctrlMode))
    {
      result = FALSE;
      break;
    }
  }

  return result;
}

STATIC FUNC(void, CANSM_CODE) CanSM_SetCtrlModes(
  const CanSM_NetworkIndexType nw,
  const CanIf_ControllerModeType ctrlMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) allModeIndicationsReceived
)
{
  uint8_least i;

  for ( i=0U; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    uint8 buf;
    TS_AtomicAssign8(buf, CanSM_CtrlModeIndication[CanSM_CtrlIdxFromCtrlId(CANSM_NW_CONFIG_CTRL(nw, i))]);

    if (buf != CanSM_ControllerModeTypeToUint8(ctrlMode))
    {
      (void)CanIf_SetControllerMode(CANSM_NW_CONFIG_CTRL(nw, i), ctrlMode);
    }
  }

  *allModeIndicationsReceived = CanSM_CheckCtrlModeIndications(nw, ctrlMode);
}

STATIC FUNC(uint8, CANSM_CODE) CanSM_ControllerModeTypeToUint8(
  const CanIf_ControllerModeType ctrlMode
)
{
  uint8 result;

  switch (ctrlMode)
  {
    case CANIF_CS_STOPPED:
      result = CANSM_CTRLMODETYPE_STOPPED;
      break;
    case CANIF_CS_SLEEP:
      result = CANSM_CTRLMODETYPE_SLEEP;
      break;
    case CANIF_CS_STARTED:
      result = CANSM_CTRLMODETYPE_STARTED;
      break;
    /* CHECK: NOPARSE */
    default:
      result = CANSM_CTRLMODETYPE_RESET;
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return result;
}

#if (CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON)
STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckCanIfGetTxConfirmationStates(
  const CanSM_NetworkIndexType nw
)
{
  boolean result = TRUE;
  uint8_least i;

  for ( i=0; i < CANSM_NW_CONFIG_CTRL_NUM(nw); i++ )
  {
    if (CanIf_GetTxConfirmationState(CANSM_NW_CONFIG_CTRL(nw, i)) != CANIF_TX_RX_NOTIFICATION)
    {
      result = FALSE;
      break;
    }
  }

  return result;
}
#endif /* CANSM_USE_CANIF_GETTXCONFIRMATIONSTATE == STD_ON */





/* transceiver helper functions */

#if (CANSM_USE_TRANSCEIVER == STD_ON)

STATIC FUNC(void, CANSM_CODE) CanSM_ClearTrcvModeIndAndSetMode(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) modeIndicationReceived
)
{
  uint8 buf = CANSM_TRCVMODETYPE_RESET;

  TS_AtomicAssign8(CANSM_NW_INFO_TRCVMODEINDICATION(nw), buf);

  CanSM_SetTrcvMode(nw, trcvMode, modeIndicationReceived);
}

STATIC FUNC(boolean, CANSM_CODE) CanSM_CheckTrcvModeIndication(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode
)
{
  boolean result = FALSE;
  uint8 buf;

  TS_AtomicAssign8(buf, CANSM_NW_INFO_TRCVMODEINDICATION(nw));

  if (buf == CanSM_TrcvModeTypeToUint8(trcvMode))
  {
    result = TRUE;
  }

  return result;
}

STATIC FUNC(void, CANSM_CODE) CanSM_SetTrcvMode(
  const CanSM_NetworkIndexType nw,
  const CanTrcv_TrcvModeType trcvMode,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) modeIndicationReceived
)
{
  (void)CanIf_SetTrcvMode(CANSM_NW_CONFIG_TRCV_ID(nw), trcvMode);

  *modeIndicationReceived = CanSM_CheckTrcvModeIndication(nw, trcvMode);
}

STATIC FUNC(uint8, CANSM_CODE) CanSM_TrcvModeTypeToUint8(
  const CanTrcv_TrcvModeType trcvMode
)
{
  uint8 result;

  switch (trcvMode)
  {
    case CANTRCV_TRCVMODE_NORMAL:
      result = CANSM_TRCVMODETYPE_NORMAL;
      break;
    case CANTRCV_TRCVMODE_STANDBY:
      result = CANSM_TRCVMODETYPE_STANDBY;
      break;
    /* CHECK: NOPARSE */
    default:
      result = CANSM_TRCVMODETYPE_RESET;
      CANSM_UNREACHABLE_CODE_ASSERT(CANSM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return result;
}

#endif /* CANSM_USE_TRANSCEIVER == STD_ON */

/* partial networking helper functions */

#if ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) )

STATIC FUNC(void, CANSM_CODE) CanSM_ClearTrcvWufFlag(
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) clearIndicationReceived
)
{
  (void)CanIf_ClearTrcvWufFlag(CANSM_NW_CONFIG_TRCV_ID(nw));

  TS_AtomicAssign8(*clearIndicationReceived, CANSM_NW_INFO_TRCVCLEARWUFINDICATION(nw));
}


STATIC FUNC(void, CANSM_CODE) CanSM_CheckTrcvWakeFlag (
  const CanSM_NetworkIndexType nw,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) checkIndicationReceived
)
{
  (void)CanIf_CheckTrcvWakeFlag(CANSM_NW_CONFIG_TRCV_ID(nw));

  TS_AtomicAssign8(*checkIndicationReceived, CANSM_NW_INFO_TRCVCHECKWAKEFLAGINDICATION(nw));
}

#endif /* ( (CANSM_PNSUPPORT == STD_ON) && (CANSM_USE_TRANSCEIVER == STD_ON) ) */

STATIC FUNC(void, CANSM_CODE) CanSM_DecrementTimerInternal (
  P2VAR(uint16, AUTOMATIC, AUTOMATIC) timer
)
{
  if (*timer > 0U)
  {
    *timer = *timer - 1U;
  }
}

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[end of file]===========================================*/
