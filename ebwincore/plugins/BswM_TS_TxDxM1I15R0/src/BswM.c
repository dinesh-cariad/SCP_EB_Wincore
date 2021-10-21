/**
 * \file
 *
 * \brief AUTOSAR BswM
 *
 * This file contains the implementation of the AUTOSAR
 * module BswM.
 *
 * \version 1.15.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO BswM.Impl.SourceFile.BswM_c,1 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>        /* Standard types                   */
#include <BswM_Trace.h>       /* Dbg macros                       */
#include <BswM_Types.h>       /* Static BswM types                */
#include <BswM.h>             /* Module public API                */
#include <BswM_Cfg.h>         /* Generated configuration          */

#if ( BSWM_DEM_ENABLED == STD_ON)
/* !LINKSTO BswM0027,2 */
#include <Dem.h>
#endif

#include <BswM_Int_Cfg.h>
#include <BswM_Int.h>         /* Internal BswM and imported types */

#include <BswM_DefProg.h>

#include <TSMem.h>            /* Used for TS_MemSet */

#if (BSWM_IS_MULTICORE_ECU == TRUE)
#include <Os.h>
#endif

/*==================[macros]================================================*/
/* !LINKSTO BswM0136,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef BSWM_VENDOR_ID /* configuration check */
#error BSWM_VENDOR_ID must be defined
#endif

#if (BSWM_VENDOR_ID != 1U) /* vendor check */
#error BSWM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef BSWM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error BSWM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (BSWM_AR_RELEASE_MAJOR_VERSION != 4U)
#error BSWM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef BSWM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error BSWM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (BSWM_AR_RELEASE_MINOR_VERSION != 0U )
#error BSWM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef BSWM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error BSWM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (BSWM_AR_RELEASE_REVISION_VERSION != 3U )
#error BSWM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef BSWM_SW_MAJOR_VERSION /* configuration check */
#error BSWM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (BSWM_SW_MAJOR_VERSION != 1U)
#error BSWM_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef BSWM_SW_MINOR_VERSION /* configuration check */
#error BSWM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (BSWM_SW_MINOR_VERSION < 15U)
#error BSWM_SW_MINOR_VERSION wrong (< 15U)
#endif

#ifndef BSWM_SW_PATCH_VERSION /* configuration check */
#error BSWM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (BSWM_SW_PATCH_VERSION < 6U)
#error BSWM_SW_PATCH_VERSION wrong (< 6U)
#endif

/*------------------[BSWM implementation macros]----------------------------*/

#if (defined BSWM_HANDLE_STATIC_REQUEST)
#error BSWM_HANDLE_STATIC_REQUEST is already defined
#endif
#define BSWM_HANDLE_STATIC_REQUEST(inst,source,channel,mode, sid) ((void)inst->LinkTimeContext->handleStaticRequestFuncPtr((channel), (mode), (source), (sid)))

#if (defined BSWM_STACK_EMPTY)
#error BSWM_STACK_EMPTY is already defined
#endif

/** \brief Evaluates if an action list stack is empty
 **
 ** \param Stack The BswMActionListStack to evaluate.
 ** \retval FALSE when the stack is not empty. */
#define BSWM_STACK_EMPTY(Stack) (((Stack).StackIndex == 0U) ? TRUE : FALSE)

#if (defined BSWM_ACTION_LIST_STACK_MAX)
#error BSWM_ACTION_LIST_STACK_MAX is already defined
#endif

/* !LINKSTO BswM0037,1 */

/** \brief The maximum height of the BswM action list execution stack + 1.
 **
 ** !description The BswM supports a maximum of 7 hierarchic levels for cascaded
 ** !            action lists. */
#define BSWM_ACTION_LIST_STACK_MAX 8U

#if (defined BSWM_NEW_ACTION_LIST_CONTEXT_STACK)
#error BSWM_NEW_ACTION_LIST_CONTEXT_STACK is already defined
#endif

/** \brief Initial value of a BswMActionListContextStackType instance. */
#define BSWM_NEW_ACTION_LIST_CONTEXT_STACK \
  {{{0U, 0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}, {0U,0U}}, 0U}
/*==================[type definitions]======================================*/

typedef struct
{
  /* The index of the next action list item to execute. */
  BswMIndexType NextItemIndex;
  /* The index of the last action list item to execute + 1. */
  BswMIndexType TailItemIndex;
} BswMActionListContextType;

typedef struct
{
  /* The action list execution contexts.  The uppermost element represents the
   * next action list execution context to use. */
  BswMActionListContextType StackElements[BSWM_ACTION_LIST_STACK_MAX];
  /* The index of the top stack element. */
  uint8 StackIndex;
} BswMActionListContextStackType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define BSWM_START_SEC_CODE
#include <BswM_MemMap.h>

/** \brief Returns the result of the logical expression.  If one or more of the
 ** associated mode request ports are in an undefined state, the value
 ** BSWM_UNDEFINED will be returned.
 **
 ** \param expPtr A pointer to a logical expression data structure.
 ** \return BSWM_TRUE if the logical expression evaluates to TRUE, BSWM_FALSE if
 ** the logical expression evaluates to FALSE and BSWM_UNDEFINED, if one or more
 ** of the associated mode conditions is in an undefined state.
 **/
STATIC FUNC(BswMResultType, BSWM_CODE) BswM_EvaluateLogicalExpression
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  BswM_IndexType exprIndex
);

/** \brief Executes the action list items of an action.
 **
 ** \param actionListPtr A pointer to an action list data structure to be
 ** executed.
 **/
STATIC FUNC(void, BSWM_CODE) BswM_ExecuteActionList
(
  P2CONST(BswMActionListType, AUTOMATIC, BSWM_CONST) actionListPtr,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
  ,P2VAR(BswM_DeferredActionsQueueType, AUTOMATIC, BSWM_VAR) deferredActionsQueue
#endif
);

/** \brief Evaluates a rule's logical expression and determines the action list
 ** to execute within the mode control phase.
 **
 ** \param rulePtr A pointer to a rule data structure to be executed.
 ** \retval The index of the action list within the ActionListTable to
 ** execute or BSWM_ARTIFACT_UNDEFINED when no action list shall be executed.
 **/
STATIC FUNC(BswMIndexType, BSWM_CODE) BswM_ExecuteRule
(
  P2CONST(BswMRuleType, AUTOMATIC, BSWM_CONST) rulePtr,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
);

/** \brief Pushes a new action list stack element onto the top of an action list
 ** stack based on the action list passed to the function.
 **
 ** \param stack The action list stack to perform a push operation on.
 ** \param actionListPtr The action list to use for initialization of the new
 ** action list stack element.
 **/
STATIC FUNC(void, BSWM_CODE) BswM_PushActionList
(
  P2CONST(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(BswMActionListContextStackType, AUTOMATIC, AUTOMATIC) stackPtr,
  P2CONST(BswMActionListType, AUTOMATIC, BSWM_CONST) actionListPtr
);

/** \brief Pushes an existing action list stack element back onto the top of an
 ** action list stack.
 **
 ** \param stack The action list stack to perform a push operation on.
 ** \param element The action list stack element to push back onto the stack.
 **/
STATIC FUNC(void, BSWM_CODE) BswM_PushActionListContext
(
  P2CONST(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(BswMActionListContextStackType, AUTOMATIC, AUTOMATIC) stackPtr,
  BswMActionListContextType element
);

/** \brief Pops the uppermost stack element off of an action list stack.
 **
 ** \param stack The action list stack to perform a pop operation on.
 ** \retval the top action list context stack element.
 **/
STATIC FUNC(BswMActionListContextType, BSWM_CODE) BswM_PopActionListContext
(
  P2VAR(BswMActionListContextStackType, AUTOMATIC, BSWM_VAR) stackPtr
);

#if (BSWM_QUEUED_ARBITRATION_ENABLED == STD_ON)
/** \brief Enqueues a mode request port
 **
 ** \param inst The BswM instance the port belongs to
 ** \param port Pointer to the mode request port object
 ** \param mode The mode the port will be updated after it is unqueued
 ** \param type The type of the mode request port
 **/
STATIC FUNC(Std_ReturnType, BSWM_CODE) BswM_EnqueueModeRequest
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(void, AUTOMATIC, BSWM_VAR) port,
  uint32 mode,
  uint8 type
);

/** \brief Dequeues a mode request port
 **
 ** \param queue Pointer to the queue the mode request port is to be unqueued from
 ** \retval Pointer to the next BswMRequestType object in the queue
 **/
STATIC FUNC_P2VAR(BswMRequestType, BSWM_VAR, BSWM_CODE) BswM_DequeueModeRequest
(
  P2VAR(BswMRequestQueueType, AUTOMATIC, BSWM_VAR) queue
);
#endif

#define BSWM_STOP_SEC_CODE
#include <BswM_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define BSWM_START_SEC_CODE
#include <BswM_MemMap.h>

#if (BSWM_VERSION_INFO_API == STD_ON)
/*============================BswM_GetVersionInfo()==========================*/

FUNC(void, BSWM_CODE) BswM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, BSWM_APPL_DATA) VersionInfoPtr
)
{
#if ((BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE) || (BSWM_DEV_ERROR_DETECT == STD_ON))
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();
#endif

  DBG_BSWM_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR )
#endif
  {
    if (NULL_PTR == VersionInfoPtr)
    {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_GET_VERSION_INFO, BSWM_E_PARAM_INVALID);
#endif
    }
    else
    {
      VersionInfoPtr->vendorID         = BSWM_VENDOR_ID;
      VersionInfoPtr->moduleID         = BSWM_MODULE_ID;
      VersionInfoPtr->sw_major_version = BSWM_SW_MAJOR_VERSION;
      VersionInfoPtr->sw_minor_version = BSWM_SW_MINOR_VERSION;
      VersionInfoPtr->sw_patch_version = BSWM_SW_PATCH_VERSION;
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_GET_VERSION_INFO, BSWM_E_WRONG_CONTEXT);
  }
#endif
  DBG_BSWM_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif /* (BSWM_VERSION_INFO_API == STD_ON) */

/*================================BswM_Deinit()===============================*/
FUNC(void, BSWM_CODE) BswM_Deinit(void)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();
  DBG_BSWM_DEINIT_ENTRY();
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
    inst->RunTimeContext.IsInitialized = FALSE;
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DEINIT, BSWM_E_WRONG_CONTEXT);
  }
#endif
  DBG_BSWM_DEINIT_EXIT();
}

#if (BSWM_GENERIC_REQUEST_API_ENABLED == STD_ON)
/*=============================BswM_RequestMode()=============================*/
FUNC(void, BSWM_CODE) BswM_RequestMode
(
  BswM_UserType requesting_user,
  BswM_ModeType requested_mode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_REQUESTMODE_ENTRY(requesting_user, requested_mode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
    BswMGenericRequestPortType* foundPort = NULL_PTR;
    if (TRUE == inst->RunTimeContext.IsInitialized)
    {
      BswM_IndexType i;
      /* For performance reasons the search for the port is done before the DET check */
      for ( i = 0U; i < inst->LinkTimeContext->numBswMGenericRequestPorts; i++)
      {
        if ( (&(inst->LinkTimeContext->genericRequestPortsTablePtr[i]))->channel == requesting_user)
        {
          foundPort = &(inst->LinkTimeContext->genericRequestPortsTablePtr[i]);
          /* All generic request ports have unique requesting users so only one can be found */
          break;
        }
      }
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_REQUESTMODE, BSWM_E_NO_INIT);
    }
    else if ( foundPort == NULL_PTR)
    {
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_REQUESTMODE, BSWM_E_REQ_USER_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    if (foundPort != NULL_PTR)
    {
      BswM_HandleRequest(inst, foundPort, requested_mode, BSWM_ARBITRATION_GENERIC_REQUEST_HANDLER_PORT, BSWM_SID_REQUESTMODE, foundPort->base.isImmediate);
    }
    else
    {
      /* ignore request */
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_REQUESTMODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_REQUESTMODE_EXIT(requesting_user, requested_mode);
}
#endif /* (BSWM_GENERIC_REQUEST_API_ENABLED == STD_ON) */

#if (BSWM_COMM_API_ENABLED == STD_ON)
/*===========================BswM_ComM_CurrentMode()==========================*/
FUNC(void, BSWM_CODE) BswM_ComM_CurrentMode
(
  NetworkHandleType Network,
  ComM_ModeType RequestedMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_COMM_CURRENTMODE_ENTRY(Network,RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(RequestedMode)
    {
      case COMM_FULL_COMMUNICATION:
      case COMM_NO_COMMUNICATION:
      case COMM_SILENT_COMMUNICATION:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_COMM_CURRENT_MODE, BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of COMM_NO_COMMUNICATION - */
    /* COMM_FULL_COMMUNICATION range */
    else if (FALSE == in_range)
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_COMM_CURRENT_MODE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_COMM_INDICATION, Network, RequestedMode, BSWM_SID_COMM_CURRENT_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_COMM_CURRENT_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_COMM_CURRENTMODE_EXIT(Network,RequestedMode);
}

/*===========================BswM_ComM_CurrentPNCMode()==========================*/
FUNC(void, BSWM_CODE) BswM_ComM_CurrentPNCMode
(
  PNCHandleType PNC,
  ComM_PncModeType RequestedMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_COMM_CURRENTPNCMODE_ENTRY(PNC, RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(RequestedMode)
    {
      case PNC_FULL_COMMUNICATION:
      case PNC_NO_COMMUNICATION:
      case PNC_REQUESTED:
      case PNC_READY_SLEEP:
      case PNC_PREPARE_SLEEP:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_COMM_CURRENT_PNC_MODE, BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of PNC_REQUESTED - */
    /* PNC_FULL_COMMUNICATION range */
    else if (FALSE == in_range)
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_COMM_CURRENT_PNC_MODE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_COMM_PNC_REQUEST, PNC, RequestedMode, BSWM_SID_COMM_CURRENT_PNC_MODE);
    }

  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_COMM_CURRENT_PNC_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_COMM_CURRENTPNCMODE_EXIT(PNC, RequestedMode);
}

/* !LINKSTO SWS_BswM_00217,1 */
FUNC(void, BSWM_CODE) BswM_ComM_InitiateReset(void)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_COMM_INITIATE_RESET, BSWM_E_NO_INIT);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      /* !LINKSTO SWS_BswM_00254,1 */
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_COMM_INITIATE_RESET, 0, BSWM_EVENT_IS_SET, BSWM_SID_COMM_INITIATE_RESET);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_COMM_INITIATE_RESET, BSWM_E_WRONG_CONTEXT);
  }
#endif
}

#endif /* (BSWM_COMM_API_ENABLED == STD_ON) */

/* !LINKSTO EB.BswM.DcmAPI,1 */
#if (BSWM_DCM_API_ENABLED == STD_ON)

/*====================BswM_Dcm_CommunicationMode_CurrentState()======================*/
FUNC(void, BSWM_CODE) BswM_Dcm_CommunicationMode_CurrentState(
  NetworkHandleType Network,
  Dcm_CommunicationModeType RequestedMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_DCM_COMMUNICATIONMODE_CURRENTSTATE_ENTRY(Network, RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE,
                            BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of DCM_ENABLE_RX_TX_NORM - */
    /* DCM_DISABLE_RX_TX_NORM_NM range */
    else if (DCM_DISABLE_RX_TX_NORM_NM < RequestedMode)
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_DCM_COM_MODE_CURRENT_STATE, Network, RequestedMode, BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_DCM_COMMUNICATIONMODE_CURRENTSTATE_EXIT(Network, RequestedMode);
}

/*======================BswM_Dcm_ApplicationUpdated()=========================*/
FUNC(void, BSWM_CODE) BswM_Dcm_ApplicationUpdated(void)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_DCM_APPLICATIONUPDATED_ENTRY();
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_APPLICATION_UPDATED, BSWM_E_NO_INIT);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_DCM_APPLICATION_UPDATED_INDICATION, 0U, 0U, BSWM_SID_DCM_APPLICATION_UPDATED);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DCM_APPLICATION_UPDATED, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_DCM_APPLICATIONUPDATED_EXIT();
}

#endif /* BSWM_DCM_API_ENABLED == STD_ON */

/* !LINKSTO EB.BswM.DcmLegacyAPI,1 */
#if (BSWM_DCM_LEGACY_API_ENABLED == STD_ON)

/*====================BswM_Dcm_RequestCommunicationMode()======================*/
FUNC(void, BSWM_CODE) BswM_Dcm_RequestCommunicationMode(
  NetworkHandleType Network,
  Dcm_CommunicationModeType RequestedMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_DCM_REQUESTCOMMUNICATIONMODE_ENTRY(Network, RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE,
                            BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of DCM_ENABLE_RX_TX_NORM - */
    /* DCM_DISABLE_RX_TX_NORM_NM range */
    else if (DCM_DISABLE_RX_TX_NORM_NM < RequestedMode)
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_DCM_COM_MODE_REQUEST, Network, RequestedMode, BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_DCM_REQUESTCOMMUNICATIONMODE_EXIT(Network, RequestedMode);
}

/*======================BswM_Dcm_RequestSessionMode()=========================*/
FUNC(void, BSWM_CODE) BswM_Dcm_RequestSessionMode(Dcm_SesCtrlType RequestedMode)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_DCM_REQUESTSESSIONMODE_ENTRY(RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
  #if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_SESSION_MODE, BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of DCM_DEFAULT_SESSION - */
    /* DCM_ALL_SESSION_LEVEL range */
    else if ((DCM_DEFAULT_SESSION > RequestedMode))
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_SESSION_MODE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
  #endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_DCM_SESSION_MODE_REQUEST, 0U, RequestedMode, BSWM_SID_DCM_REQUEST_SESSION_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DCM_REQUEST_SESSION_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_DCM_REQUESTSESSIONMODE_EXIT(RequestedMode);
}

/*=======================BswM_Dcm_RequestResetMode()===========================*/
FUNC(void, BSWM_CODE) BswM_Dcm_RequestResetMode(Dcm_ResetModeType RequestedMode)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_DCM_REQUESTRESETMODE_ENTRY(RequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_RESET_MODE, BSWM_E_NO_INIT);
    }
    /* Check if requested mode is out of DCM_NO_RESET - */
    /* DCM_RESET_EXECUTION range */
    else if (DCM_RESET_EXECUTION < RequestedMode)
    {
      /* Report error ,that requested mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_DCM_REQUEST_RESET_MODE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_DCM_RESET_MODE_REQUEST, 0U, RequestedMode, BSWM_SID_DCM_REQUEST_RESET_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_DCM_REQUEST_RESET_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_DCM_REQUESTRESETMODE_EXIT(RequestedMode);
}

#endif /* (BSWM_DCM_LEGACY_API_ENABLED == STD_ON) */

#if (BSWM_CANSM_API_ENABLED == STD_ON)
/*=======================BswM_CanSM_CurrentState()=============================*/
FUNC(void, BSWM_CODE) BswM_CanSM_CurrentState
(
  NetworkHandleType Network,
  CanSM_BswMCurrentStateType CurrentState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_CANSM_CURRENTSTATE_ENTRY(Network, CurrentState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(CurrentState)
    {
      case CANSM_BSWM_NO_COMMUNICATION:
      case CANSM_BSWM_SILENT_COMMUNICATION:
      case CANSM_BSWM_FULL_COMMUNICATION:
#if (defined CANSM_BSWM_BUS_OFF_L1)
      case CANSM_BSWM_BUS_OFF_L1:
#endif
#if (defined CANSM_BSWM_BUS_OFF_L2)
      case CANSM_BSWM_BUS_OFF_L2:
#endif
      case CANSM_BSWM_BUS_OFF:
      case CANSM_BSWM_CHANGE_BAUDRATE:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_CANSM_CURRENT_STATE, BSWM_E_NO_INIT);
    }
    /* Check if Current State of CAN channel is out of range */
    else if (FALSE == in_range)
    {
      /* Report error ,that Current State of CAN channel is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_CANSM_CURRENT_STATE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_CAN_SM_INDICATION, Network, CurrentState, BSWM_SID_CANSM_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_CANSM_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_CANSM_CURRENTSTATE_EXIT(Network, CurrentState);
}
#endif /* (BSWM_CANSM_API_ENABLED == STD_ON) */

#if (BSWM_ETHIF_API_ENABLED == STD_ON)
/* !LINKSTO SWS_BswM_91001,1 */
FUNC(void, BSWM_CODE) BswM_EthIf_PortGroupLinkStateChg
(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  EthTrcv_LinkStateType PortGroupState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_ETHIF_PORTGROUPLINKSTATECHG_ENTRY(PortGroupIdx, PortGroupState);

  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* !LINKSTO SWS_BswM_00267,1 */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG, BSWM_E_NO_INIT);
    }
    else if ( (PortGroupState != ETHTRCV_LINK_STATE_DOWN) &&
              (PortGroupState != ETHTRCV_LINK_STATE_ACTIVE)
            )
    {
      /* !LINKSTO SWS_BswM_00268,1 */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_ETHIF_PORT_GROUP_LINK_STATE_CHANGED, PortGroupIdx, PortGroupState, BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG, BSWM_E_WRONG_CONTEXT);
  }
#endif
  DBG_BSWM_ETHIF_PORTGROUPLINKSTATECHG_EXIT(PortGroupIdx, PortGroupState);
}
#endif

#if (BSWM_ETHSM_API_ENABLED == STD_ON)
/*=======================BswM_EthSM_CurrentState()=============================*/
FUNC(void, BSWM_CODE) BswM_EthSM_CurrentState
(
  NetworkHandleType Network,
  EthSM_NetworkModeStateType CurrentState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_ETHSM_CURRENTSTATE_ENTRY(Network, CurrentState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(CurrentState)
    {
      case ETHSM_BSWM_OFFLINE:
      case ETHSM_BSWM_WAIT_TRCVLINK:
      case ETHSM_BSWM_WAIT_ONLINE:
      case ETHSM_BSWM_ONLINE:
      case ETHSM_BSWM_ONHOLD:
      case ETHSM_BSWM_WAIT_OFFLINE:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ETHSM_CURRENT_STATE, BSWM_E_NO_INIT);
    }
    /* Check if Current State of Ethernet channel is out of ETHSM_BSWM_OFFLINE - */
    /* ETHSM_BSWM_WAIT_OFFLINE enum range */
    else if (FALSE == in_range)
    {
      /* Report error ,that Current State of Ethernet channel */
      /* is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ETHSM_CURRENT_STATE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_ETH_SM_INDICATION, Network, CurrentState, BSWM_SID_ETHSM_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_ETHSM_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_ETHSM_CURRENTSTATE_EXIT(Network, CurrentState);
}
#endif /* (BSWM_ETHSM_API_ENABLED == STD_ON) */

#if (BSWM_FRSM_API_ENABLED == STD_ON)
/*=======================BswM_FrSM_CurrentState()=============================*/
FUNC(void, BSWM_CODE) BswM_FrSM_CurrentState
(
  NetworkHandleType Network,
  FrSM_BswM_StateType CurrentState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_FRSM_CURRENTSTATE_ENTRY(Network, CurrentState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(CurrentState)
    {
      case FRSM_BSWM_READY:
      case FRSM_BSWM_READY_ECU_PASSIVE:
      case FRSM_BSWM_STARTUP:
      case FRSM_BSWM_STARTUP_ECU_PASSIVE:
      case FRSM_BSWM_WAKEUP:
      case FRSM_BSWM_WAKEUP_ECU_PASSIVE:
      case FRSM_BSWM_HALT_REQ:
      case FRSM_BSWM_HALT_REQ_ECU_PASSIVE:
      case FRSM_BSWM_KEYSLOT_ONLY:
      case FRSM_BSWM_KEYSLOT_ONLY_ECU_PASSIVE:
      case FRSM_BSWM_ONLINE:
      case FRSM_BSWM_ONLINE_ECU_PASSIVE:
      case FRSM_BSWM_ONLINE_PASSIVE:
      case FRSM_BSWM_ONLINE_PASSIVE_ECU_PASSIVE:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_FRSM_CURRENT_STATE, BSWM_E_NO_INIT);
    }
    /* Check if Current State of Flexray channel is out of FRSM_BSWM_READY - */
    /* FRSM_BSWM_ONLINE_PASSIVE_ECU_PASSIVE range */
    else if (FALSE == in_range)
    {
      /* Report error ,that Current State of Flexray channel */
      /* is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_FRSM_CURRENT_STATE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_FR_SM_INDICATION, Network, CurrentState, BSWM_SID_FRSM_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_FRSM_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_FRSM_CURRENTSTATE_EXIT(Network, CurrentState);
}
#endif /* (BSWM_FRSM_API_ENABLED == STD_ON) */

#if (BSWM_LINSM_API_ENABLED == STD_ON)
/*=======================BswM_LinSM_CurrentState()=============================*/
FUNC(void, BSWM_CODE) BswM_LinSM_CurrentState
(
  NetworkHandleType Network,
  LinSM_ModeType CurrentState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_LINSM_CURRENTSTATE_ENTRY(Network, CurrentState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINSM_CURRENT_STATE, BSWM_E_NO_INIT);
    }
    /* Check if Current State of LIN channel is out of configured range */
    else if ((LINSM_NO_COM != CurrentState) && (LINSM_FULL_COM != CurrentState))
    {
      /* Report error ,that Current State of LIN channel */
      /* is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINSM_CURRENT_STATE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_LIN_SM_INDICATION, Network , CurrentState, BSWM_SID_LINSM_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_LINSM_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_LINSM_CURRENTSTATE_EXIT(Network, CurrentState);
}
#endif /* (BSWM_LINSM_API_ENABLED == STD_ON) */

#if (BSWM_ECUM_API_ENABLED == STD_ON)
/*=======================BswM_EcuM_CurrentState()==============================*/
FUNC(void, BSWM_CODE) BswM_EcuM_CurrentState
(
  EcuM_StateType CurrentState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_ECUM_CURRENTSTATE_ENTRY(CurrentState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;
    switch(CurrentState)
    {
      case ECUM_STATE_RESET:
      case ECUM_SUBSTATE_MASK:
      case ECUM_STATE_STARTUP:
      case ECUM_STATE_STARTUP_ONE:
      case ECUM_STATE_STARTUP_TWO:
      case ECUM_STATE_WAKEUP:
      case ECUM_STATE_WAKEUP_VALIDATION:
      case ECUM_STATE_WAKEUP_REACTION:
      case ECUM_STATE_WAKEUP_TWO:
      case ECUM_STATE_WAKEUP_WAKESLEEP:
      case ECUM_STATE_WAKEUP_TTII:
      case ECUM_STATE_RUN:
      case ECUM_STATE_APP_RUN:
      case ECUM_STATE_APP_POST_RUN:
      case ECUM_STATE_SHUTDOWN:
      case ECUM_STATE_PREP_SHUTDOWN:
      case ECUM_STATE_GO_SLEEP:
      case ECUM_STATE_GO_OFF_ONE:
      case ECUM_STATE_GO_OFF_TWO:
      case ECUM_STATE_SLEEP:
      case ECUM_STATE_OFF:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ECUM_CURRENT_STATE, BSWM_E_NO_INIT);
    }
    /* Check if Current State of EcuM is out of ECUM_SUBSTATE_MASK - */
    /* ECUM_STATE_RESET range */
    else if (FALSE == in_range)
    {
      /* Report error ,that Current State of EcuM is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ECUM_CURRENT_STATE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_ECUM_INDICATION, 0U, CurrentState, BSWM_SID_ECUM_CURRENT_STATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_ECUM_CURRENT_STATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_ECUM_CURRENTSTATE_EXIT(CurrentState);
}

/*=======================BswM_EcuM_CurrentWakeup()============================*/
FUNC(void, BSWM_CODE) BswM_EcuM_CurrentWakeup
(
  EcuM_WakeupSourceType source,
  EcuM_WakeupStatusType state
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_ECUM_CURRENTWAKEUP_ENTRY(source, state);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ECUM_CURRENT_WAKEUP, BSWM_E_NO_INIT);
    }
    /* Check if state of EcuM is out of ECUM_WKSTATUS_NONE - */
    /* ECUM_WKSTATUS_DISABLED range */
    else if (ECUM_WKSTATUS_DISABLED < state)
    {
      /* Report error ,that state of EcuM is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_ECUM_CURRENT_WAKEUP,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_ECUM_WAKEUP_SOURCE, source, state, BSWM_SID_ECUM_CURRENT_WAKEUP);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_ECUM_CURRENT_WAKEUP, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_ECUM_CURRENTWAKEUP_EXIT(source, state);
}
#endif /* (BSWM_ECUM_API_ENABLED == STD_ON) */

#if (BSWM_NM_API_ENABLED == STD_ON)
/* !LINKSTO SWS_BswM_00235,1 */
FUNC(void, BSWM_CODE) BswM_NmIf_CarWakeUpIndication
(
  NetworkHandleType Network
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_NMIF_CARWAKEUPINDICATION_ENTRY(Network);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_NM_CAR_WAKEUP_INDICATION, BSWM_E_NO_INIT);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_NM_CAR_WAKEUP_INDICATION, Network, 0U, BSWM_SID_NM_CAR_WAKEUP_INDICATION);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_NM_CAR_WAKEUP_INDICATION, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_NMIF_CARWAKEUPINDICATION_EXIT(Network);
}
#endif

#if (BSWM_NVM_API_ENABLED == STD_ON)
/*=======================BswM_NvM_CurrentBlockMode()==========================*/
FUNC(void, BSWM_CODE) BswM_NvM_CurrentBlockMode
(
  NvM_BlockIdType Block,
  NvM_RequestResultType CurrentBlockMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_NVM_CURRENTBLOCKMODE_ENTRY(Block, CurrentBlockMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_NVM_CURRENT_BLOCK_MODE, BSWM_E_NO_INIT);
    }
    else if ( NVM_REQ_RESTORED_FROM_ROM < CurrentBlockMode)
    {
      /* Report error ,that CurrentBlockMode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_NVM_CURRENT_BLOCK_MODE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_NVM_REQUEST, Block, CurrentBlockMode, BSWM_SID_NVM_CURRENT_BLOCK_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_NVM_CURRENT_BLOCK_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_NVM_CURRENTBLOCKMODE_EXIT(Block, CurrentBlockMode);
}

/*=======================BswM_NvM_CurrentJobMode()==========================*/
FUNC(void, BSWM_CODE) BswM_NvM_CurrentJobMode
(
  uint8 ServiceId,
  NvM_RequestResultType CurrentJobMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_NVM_CURRENTJOBMODE_ENTRY(ServiceId, CurrentJobMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_NVM_CURRENT_JOB_MODE, BSWM_E_NO_INIT);
    }
    else if ( NVM_REQ_RESTORED_FROM_ROM < CurrentJobMode)
    {
      /* Report error ,that ServiceId is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_NVM_CURRENT_JOB_MODE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_NVM_JOB_MODE_INDICATION, ServiceId, CurrentJobMode, BSWM_SID_NVM_CURRENT_JOB_MODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_NVM_CURRENT_JOB_MODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_NVM_CURRENTJOBMODE_EXIT(ServiceId, CurrentJobMode);
}

#endif /* (BSWM_NVM_API_ENABLED == STD_ON) */

#if (BSWM_LINSM_API_ENABLED == STD_ON)
/*=======================BswM_LinSM_CurrentSchedule()=========================*/
FUNC(void, BSWM_CODE) BswM_LinSM_CurrentSchedule
(
    NetworkHandleType Network,
    LinIf_SchHandleType CurrentSchedule
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_LINSM_CURRENTSCHEDULE_ENTRY(Network, CurrentSchedule);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINSM_CURRENT_SCHEDULE, BSWM_E_NO_INIT);
    }
    /* Check if currently active schedule table of the LIN */
    /* channel is out of configured range */
    else if (BSWM_LINIF_SCHEDULE_RANGE_CHECK)
    {
      /* Report error ,that currently active schedule table of the LIN */
      /* channel is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINSM_CURRENT_SCHEDULE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_LIN_SCHEDULE_INDICATION, Network, CurrentSchedule, BSWM_SID_LINSM_CURRENT_SCHEDULE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_LINSM_CURRENT_SCHEDULE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_LINSM_CURRENTSCHEDULE_EXIT(Network, CurrentSchedule);
}
#endif /* (BSWM_LINSM_API_ENABLED == STD_ON) */

#if (BSWM_LINTP_API_ENABLED == STD_ON)
/*=======================BswM_LinTp_RequestMode()=============================*/
FUNC(void, BSWM_CODE) BswM_LinTp_RequestMode
(
  NetworkHandleType Network,
  LinTp_Mode LinTpRequestedMode
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_LINTP_REQUESTMODE_ENTRY(Network, LinTpRequestedMode);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    boolean in_range;

    switch(LinTpRequestedMode)
    {
      case LINTP_DIAG_RESPONSE:
      case LINTP_APPLICATIVE_SCHEDULE:
      case LINTP_DIAG_REQUEST:
      {
        in_range = TRUE;
        break;
      }
      default:
      {
        in_range = FALSE;
        break;
      }
    }

    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINTP_REQUESTMODE, BSWM_E_NO_INIT);
    }
    /* Check if requested LIN Tp mode is out of configured range */
    else if (FALSE == in_range)
    {
      /* Report error ,that requested LIN Tp mode is not in range, to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_LINTP_REQUESTMODE, BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_LIN_TP_MODE_REQUEST, Network, LinTpRequestedMode, BSWM_SID_LINTP_REQUESTMODE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_LINTP_REQUESTMODE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_LINTP_REQUESTMODE_EXIT(Network, LinTpRequestedMode);
}
#endif /* (BSWM_LINTP_API_ENABLED == STD_ON) */

#if (BSWM_SD_API_ENABLED == STD_ON)
/*=======================BswM_Sd_ClientServiceCurrentState()=============================*/
FUNC(void, BSWM_CODE) BswM_Sd_ClientServiceCurrentState
(
  uint16 SdClientServiceHandleId,
  Sd_ClientServiceCurrentStateType CurrentClientState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_SD_CLIENTSERVICECURRENTSTATE_ENTRY(SdClientServiceHandleId, CurrentClientState);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE, BSWM_E_NO_INIT);
    }
    /* Check if indicated Sd state is out of configured range */
    else if ((SD_CLIENT_SERVICE_DOWN != CurrentClientState) &&
              (SD_CLIENT_SERVICE_AVAILABLE != CurrentClientState))
    {
      /* Report error that indicated Sd state is not in range to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_SD_CLIENT_SERVICE_CURRENT_STATE, SdClientServiceHandleId, CurrentClientState, BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_SD_CLIENTSERVICECURRENTSTATE_EXIT(SdClientServiceHandleId, CurrentClientState);
}

/*=======================BswM_Sd_ConsumedEventGroupCurrentState()=============*/
FUNC(void, BSWM_CODE) BswM_Sd_ConsumedEventGroupCurrentState
(
  uint16 SdConsumedEventGroupHandleId,
  Sd_ConsumedEventGroupCurrentStateType ConsumedEventGroupState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_SD_CONSUMEDEVENTGROUPCURRENTSTATE_ENTRY
  (
    SdConsumedEventGroupHandleId,
    ConsumedEventGroupState
  );
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE, BSWM_E_NO_INIT);
    }
    /* Check if indicated Sd state is out of configured range */
    else if ((SD_CONSUMED_EVENTGROUP_DOWN != ConsumedEventGroupState) &&
              (SD_CONSUMED_EVENTGROUP_AVAILABLE != ConsumedEventGroupState))
    {
      /* Report error that indicated Sd state is not in range to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_SD_CONSUMED_EVENT_GROUP_CURRENT_STATE, SdConsumedEventGroupHandleId, ConsumedEventGroupState, BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_SD_CONSUMEDEVENTGROUPCURRENTSTATE_EXIT
  (
    SdConsumedEventGroupHandleId,
    ConsumedEventGroupState
  );
}

/*=======================BswM_Sd_EventHandlerCurrentState()===================*/
FUNC(void, BSWM_CODE) BswM_Sd_EventHandlerCurrentState
(
  uint16 SdEventHandlerHandleId,
  Sd_EventHandlerCurrentStateType EventHandlerStatus
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_SD_EVENTHANDLERCURRENTSTATE_ENTRY(SdEventHandlerHandleId, EventHandlerStatus);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE, BSWM_E_NO_INIT);
    }
    /* Check if indicated Sd state is out of configured range */
    else if ((SD_EVENT_HANDLER_RELEASED != EventHandlerStatus) &&
              (SD_EVENT_HANDLER_REQUESTED != EventHandlerStatus))
    {
      /* Report error that indicated Sd state is not in range to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE,
                            BSWM_E_REQ_MODE_OUT_OF_RANGE);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_SD_EVENT_HANDLER_CURRENT_STATE, SdEventHandlerHandleId, EventHandlerStatus, BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_SD_EVENTHANDLERCURRENTSTATE_EXIT(SdEventHandlerHandleId, EventHandlerStatus);
}
#endif /* (BSWM_SD_API_ENABLED == STD_ON) */

#if (BSWM_WDGM_API_ENABLED == STD_ON)
/*====================BswM_WdgM_RequestPartitionReset()========================*/
FUNC(void, BSWM_CODE) BswM_WdgM_RequestPartitionReset
(
  ApplicationType Application
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_WDGM_REQUESTPARTITIONRESET_ENTRY(Application);
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_WDGM_REQUEST_PARTITION_RESET, BSWM_E_NO_INIT);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_WDGM_REQUEST_PARTITION_RESET, Application, 0U, BSWM_SID_WDGM_REQUEST_PARTITION_RESET);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_WDGM_REQUEST_PARTITION_RESET, BSWM_E_WRONG_CONTEXT);
  }
#endif

  DBG_BSWM_WDGM_REQUESTPARTITIONRESET_EXIT(Application);
}
#endif /* (BSWM_WDGM_API_ENABLED == STD_ON) */

#if (BSWM_J1939DCM_API_ENABLED == STD_ON)
/* !LINKSTO SWS_BswM_00165,1 */
FUNC(void, BSWM_CODE) BswM_J1939DcmBroadcastStatus
(
  uint16 NetworkMask
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    /* Check if module is NOT initialized */
    if (FALSE == inst->RunTimeContext.IsInitialized)
    {
      /* Report error that module is not initialized to DET */
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_J1939DCMBROADCASTSTATUS, BSWM_E_NO_INIT);
    }
    else
#endif /* #if (BSWM_DEV_ERROR_DETECT == STD_ON) */
    {
      BSWM_HANDLE_STATIC_REQUEST(inst, BSWM_J1939_DCM_BROADCAST_STATUS, NetworkMask, 0U, BSWM_SID_J1939DCMBROADCASTSTATUS);
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_J1939DCMBROADCASTSTATUS, BSWM_E_WRONG_CONTEXT);
  }
#endif
}
#endif

#if (BSWM_J1939NM_API_ENABLED == STD_ON)
/* !LINKSTO SWS_BswM_00194,1 */
FUNC(void, BSWM_CODE) BswM_J1939Nm_StateChangeNotification
(
  NetworkHandleType Network,
  uint8 Node,
  Nm_StateType NmState
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
    if (TRUE == inst->RunTimeContext.IsInitialized)
    {
      BswMJ1939NmStateChangeNotificationPortType* foundPort = NULL_PTR;
      BswM_IndexType i;

      for ( i = 0U; i < inst->LinkTimeContext->numBswMJ1939NmPorts; i++)
      {
        if ( (&(inst->LinkTimeContext->j1939NmRequestPortsTablePtr[i]))->channel == (((uint16)Network << 8U) + Node))
        {
          foundPort = &(inst->LinkTimeContext->j1939NmRequestPortsTablePtr[i]);
          break;
        }
      }

      if (foundPort != NULL_PTR)
      {
        BswM_HandleRequest(inst, foundPort, NmState, BSWM_ARBITRATION_J1939NM_HANDLER_PORT, BSWM_SID_J1939NM_STATECHANGENOTIFICATION, foundPort->base.isImmediate);
      }
    }
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      BSWM_DET_REPORT_ERROR(inst->ID, BSWM_SID_J1939NM_STATECHANGENOTIFICATION, BSWM_E_NO_INIT);
    }
#endif
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_J1939NM_STATECHANGENOTIFICATION, BSWM_E_WRONG_CONTEXT);
  }
#endif
}
#endif

/*===========================BswM_MainFunction()=============================*/

FUNC(void, BSWM_CODE) BswM_MainFunction(void)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst = BswM_GetInstance();

  DBG_BSWM_MAINFUNCTION_ENTRY();
  /* !LINKSTO BswM.Impl.RunTime.InvalidPartitionContext,3 */
#if (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE)
  if ( inst != NULL_PTR)
#endif
  {
    if (FALSE != inst->RunTimeContext.IsInitialized)
    {
#if (BSWM_QUEUED_ARBITRATION_ENABLED == STD_ON)
#if (BSWM_DELAY_MAINFUNCTION == STD_OFF)
       boolean processQueuedRequests = FALSE;
#endif
       inst->SchMEnter();
       if (inst->RunTimeContext.BswMRequestQueue.active == FALSE)
       {
#if (BSWM_DELAY_MAINFUNCTION == STD_OFF)
         processQueuedRequests = TRUE;
#endif
         inst->RunTimeContext.BswMRequestQueue.active = TRUE;
       }
/* !LINKSTO BswM.Impl.BswMQueuedArbitration.Immediate.MainFunction.Allow,1 */
#if (BSWM_DELAY_MAINFUNCTION == STD_ON)
       else
       {
         inst->RunTimeContext.MainFunctionDelayed = TRUE;
         inst->SchMExit();
         /* !LINKSTO BswM.Impl.BswMQueuedArbitration.Immediate.MainFunction.Return,1 */
         return;
       }
#endif
       inst->SchMExit();
#endif
      {
#if ( BSWM_TIMER_CONTROL_ACTIONS_USED == STD_ON)
        /* Process the timers before the deferred mode arbitration
           in order for rules depending on them to be evaluated in
           this cycle if they expire
         */
        {
          uint16 i;
          boolean timerTable[BSWM_TIMER_TABLE_SIZE];
          boolean hasExpiredTimers = FALSE;
          TS_MemSet(timerTable, FALSE, BSWM_TIMER_TABLE_SIZE);

          inst->SchMEnter();
          for (i = 0U; i < inst->LinkTimeContext->numTimers; i++)
          {
            if ( inst->RunTimeContext.TimerTable[i].state == BSWM_TIMER_STARTED)
            {
              /* !LINKSTO SWS_BswM_00263,1 */
              BSWM_PRECONDITION_ASSERT(inst->ID, inst->RunTimeContext.TimerTable[i].value > 0U, BSWM_INTERNAL_API_ID);
              inst->RunTimeContext.TimerTable[i].value--;
              if ( inst->RunTimeContext.TimerTable[i].value == 0U)
              {
                inst->RunTimeContext.TimerTable[i].state = BSWM_TIMER_EXPIRED;
                timerTable[i] = TRUE;
                hasExpiredTimers = TRUE;
              }
            }
          }
          inst->SchMExit();
          if (hasExpiredTimers == TRUE)
          {
            for (i = 0U; i < BSWM_TIMER_TABLE_SIZE; i++)
            {
              if (timerTable[i] == TRUE)
              {
                /* !LINKSTO SWS_BswM_00264,1 */
                /* Mode arbitration already runs in an exclusive area */
                BswM_ExecuteModeArbitration(inst, &(inst->LinkTimeContext->timersTablePtr[i]), BSWM_TIMER_EXPIRED, BSWM_ARBITRATION_STATIC_HANDLER_PORT);
              }
            }
          }
        }
#endif
        /* Deferred mode arbitration */
        {
          BswM_IndexType i;
          BswMIndexType actionListIndex;
          boolean hasActivatedActionLists = FALSE;
          boolean activatedActionLists[BSWM_MAX_NUM_ACTION_LISTS];
          TS_MemSet(activatedActionLists, 0x00U, BSWM_MAX_NUM_ACTION_LISTS);
          inst->SchMEnter();
          /* Rules with deferred evaluation are always at the beginning of the RuleTable */
          for (i = 0U; i < BSWM_GET_NUM_RULES_DEFERRED(inst->ID); i++)
          {
            actionListIndex = BswM_ExecuteRule(&BSWM_GET_RULE_TABLE(inst->ID)[i], inst);
            /* If no action list shall be executed, BswM_ExecuteRule shall return
             * the index BSWM_ARTIFACT_UNDEFINED. */
            if (actionListIndex != BSWM_ARTIFACT_UNDEFINED)
            {
              hasActivatedActionLists = TRUE;
              activatedActionLists[actionListIndex] = TRUE;
            }
            else
            {
              /* No action list shall be triggered. */
            }
          }
          inst->SchMExit();

          if ( hasActivatedActionLists == TRUE )
          {
            /* Mode Control */
            BswM_ExecuteModeControl(activatedActionLists, inst);
          }
        }
/* !LINKSTO BswM.Impl.BswMQueuedArbitration.Dequeue.MainFunction,1 */
#if (BSWM_QUEUED_ARBITRATION_ENABLED == STD_ON)
#if (BSWM_DELAY_MAINFUNCTION == STD_OFF)
        if (processQueuedRequests == TRUE)
#endif
        /* Process requests which interrupted the BswM_MainFunction */
        {
          BswMRequestType* request;
          inst->SchMEnter();
          while((request = BswM_DequeueModeRequest(&(inst->RunTimeContext.BswMRequestQueue))) != NULL_PTR)
          {
            inst->SchMExit();
            BswM_ExecuteModeArbitration(inst, request->port, request->mode, request->type);
            inst->SchMEnter();
          }

          inst->RunTimeContext.BswMRequestQueue.active = FALSE;
          inst->SchMExit();
        }
#endif
      }
    }
    else
    {
      /* Mode Arbitration/Mode Control shall not be carried out. */
    }
  }
#if ((BSWM_DEV_ERROR_DETECT == STD_ON) && (BSWM_GET_INSTANCE_BSW_DISTRIBUTION == TRUE))
  else
  {
    BSWM_DET_REPORT_ERROR(BSWM_GET_APPLICATION_ID(), BSWM_SID_MAINFUNCTION, BSWM_E_WRONG_CONTEXT);
  }
#endif
  DBG_BSWM_MAINFUNCTION_EXIT();
}

FUNC(boolean, BSWM_CODE) BswM_PortUpdated
(
  P2VAR(BswMBasicPortType, AUTOMATIC, BSWM_VAR) portPtr,
  P2VAR(boolean, AUTOMATIC, AUTOMATIC) activatedActionLists,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
)
{
  boolean retVal = FALSE;
  /* The index of a rule-dependency within a RuleDependencyTable
   * segment. The value of RuleDependencyTable[dependIndex] will always
   * be the index of a rule within the RuleTable. */
  BswMIndexType dependIndex;
  /* The index of an action list within the ActionListTable. */
  BswMIndexType actionListIndex;

  /* The mode request port has been updated at least once. */
  portPtr->isDefined = TRUE;

  /* If the mode request port has immediate evaluation, perform mode arbitration
   * now.  Otherwise, wait until the BswM_MainFunction is executed. */
  if (TRUE == portPtr->isImmediate)
  {
    /* Find all of the dependent rules and execute them.  Since the dependent
     * rules are not known at link-time, the post-build array segment
     * RuleDependencyTable[Head..Tail] contains the indexes of the
     * dependent rules for this mode request port. */
    /* If Head == Tail, no rules depend on this mode request port. */
    for
    (
      dependIndex = BSWM_GET_RULE_DEPENDENCY_LIST_RANGE_TABLE(inst->ID)[portPtr->id].Head;
      dependIndex < BSWM_GET_RULE_DEPENDENCY_LIST_RANGE_TABLE(inst->ID)[portPtr->id].Tail;
      dependIndex++
    )
    {
      /* !LINKSTO BswM0059,1 */
      /* Only the BswM rules that that use a specific immediate
       * mode condition will be evaluated by the BswM within the
       * context of that specific mode request/indication. */
      actionListIndex =
        BswM_ExecuteRule(&BSWM_GET_RULE_TABLE(inst->ID)[BSWM_GET_RULE_DEPENDENCY_TABLE(inst->ID)[dependIndex]], inst);
      /* If no action list shall be executed, BswM_ExecuteRule shall return the
       * index BSWM_ARTIFACT_UNDEFINED. */
      if (actionListIndex != BSWM_ARTIFACT_UNDEFINED)
      {
        activatedActionLists[actionListIndex] = TRUE;
        retVal = TRUE;
      }
      else
      {
        /* No action list shall be triggered. */
      }
    }
  }
  return retVal;
}

STATIC FUNC(BswMResultType, BSWM_CODE) BswM_EvaluateLogicalExpression
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  BswM_IndexType exprIndex
)
{
  BswMResultType result;

  if (FALSE == BSWM_GET_LOG_EXPR_INIT_STATUS(exprIndex))
  {
    /* Re-evaluate if the expression is now initialized. */
    BSWM_SET_LOG_EXPR_INIT_STATUS(exprIndex, BSWM_GET_EXPR_STATE(exprIndex));
  }
  else
  {
    /* Expression already set to initialized. State does not need to be
     * updated. */
  }

  if (TRUE == BSWM_GET_LOG_EXPR_INIT_STATUS(exprIndex))
  {
    /* The expression is now in an initialized state, get the expression's
     * result. */
    result = BSWM_GET_EXPR_RESULT(exprIndex);
  }
  else
  {
    /* The expression still has not reached an initialized state, notify the
     * caller that the expression's result cannot be used. */
    result = BSWM_UNDEFINED;
  }

  return result;
}

FUNC(void, BSWM_CODE) BswM_ExecuteModeControl
(
  P2CONST(boolean, AUTOMATIC, AUTOMATIC) activatedActionLists,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
)
{
  BswMIndexType i;
#if (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON)
  uint8 pduGroupSwitchTriggered = FALSE;
  uint8 pduGroupSwitchReInitTriggered = FALSE;
#endif
#if (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON)
  uint8 deadlineMonitoringControlTriggered = FALSE;
#endif
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
  BswM_DeferredActionsQueueType deferredActionsQueue;
  deferredActionsQueue.size = 0;
  for(i = 0U; i < BSWM_DEFERRED_ACTIONS_QUEUE_SIZE; i++)
  {
    deferredActionsQueue.data[i] = 0U;
  }
#endif

  for (i = 0U; i < BSWM_GET_NUM_ACTION_LISTS(inst->ID); i++)
  {
    if (TRUE == activatedActionLists[i])
    {
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
      BswM_ExecuteActionList(&BSWM_GET_ACTION_LIST_TABLE(inst->ID)[i], inst, &deferredActionsQueue);
#else
      BswM_ExecuteActionList(&BSWM_GET_ACTION_LIST_TABLE(inst->ID)[i], inst);
#endif
    }
    else
    {
      /* Action list not activated, don't execute it. */
    }
  }

  /*
   * Execution of Pdu Group Switch and Deadline Monitoring Control actions
   * is delayed at the end of the mode control.
   */
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
  inst->SchMEnter();
  for(i = 0U; i < deferredActionsQueue.size; i++)
  {
    (void) BSWM_EXECUTE_ACTION(deferredActionsQueue.data[i]);
  }
  inst->SchMExit();
#endif

#if (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON)
  inst->SchMEnter();
  pduGroupSwitchTriggered = inst->RunTimeContext.PduGroupSwitchTriggered;
  pduGroupSwitchReInitTriggered = inst->RunTimeContext.PduGroupSwitchReInitTriggered;
  inst->SchMExit();
#endif

#if (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON)
  inst->SchMEnter();
  deadlineMonitoringControlTriggered = inst->RunTimeContext.DeadlineMonitoringControlTriggered;
  inst->SchMExit();
#endif

#if (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON)
  /* Execution of the Com_IpduGroupControl command is delayed to
   * the end of the BswM main function or immediate request. */
  if (TRUE == pduGroupSwitchReInitTriggered)
  {
    inst->SchMEnter();
    inst->RunTimeContext.PduGroupSwitchReInitTriggered = FALSE;
    inst->SchMExit();
    /* !LINKSTO BswM0129_Refine_2,1 */
    /* !LINKSTO BswM0129_Refine_3,1 */
    Com_IpduGroupControl(inst->RunTimeContext.IPduGroupReInitVector, TRUE);
  }
  if (TRUE == pduGroupSwitchTriggered)
  {
    inst->SchMEnter();
    inst->RunTimeContext.PduGroupSwitchTriggered = FALSE;
    inst->SchMExit();
    /* !LINKSTO BswM0129_Refine_1,1 */
    Com_IpduGroupControl(inst->RunTimeContext.IPduGroupVector, FALSE);

    /*
     * After the Com_IpduGroupControl is called the IPduGroupReInitVector must be
     * synchronized with the IPduGroupVector in order to ensure future consistency.
     */
    /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule5,1 */
    inst->SchMEnter();
    for (i = 0U; i < BSWM_IPDU_GROUP_VECTOR_SIZE; i++)
    {
      inst->RunTimeContext.IPduGroupReInitVector[i] |= inst->RunTimeContext.IPduGroupVector[i];
    }
    inst->SchMExit();
  }
#endif
#if (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON)
  if (TRUE == deadlineMonitoringControlTriggered)
  {
    inst->SchMEnter();
    inst->RunTimeContext.DeadlineMonitoringControlTriggered = FALSE;
    inst->SchMExit();
    /* !LINKSTO SWS_BswM_00225,1 */
    Com_ReceptionDMControl(inst->RunTimeContext.DmcIPduGroupVector);
  }
#endif
}

#if (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON)
FUNC(void, BSWM_CODE) BswM_ExecutePduGroupSwitchAction
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  BswMIndexType pbIndex
)
{
  uint16 i;
  P2CONST(BswMPduGroupSwitchActionType, AUTOMATIC, BSWM_CONST) action = &(BSWM_GET_PDU_GROUP_SWITCH_ACTION_TABLE(inst->ID)[pbIndex]);

  if ( TRUE == action->RequestInit )
  {
    inst->RunTimeContext.PduGroupSwitchReInitTriggered = TRUE;
  }
  else
  {
    inst->RunTimeContext.PduGroupSwitchTriggered = TRUE;
  }

  for (i = 0U; i < BSWM_IPDU_GROUP_VECTOR_SIZE; i++)
  {
    /*
     * A clear operation performed on the IPduGroupVector must trigger a corresponding clear operation
     * on the IPduGroupReInitVector.
     * A set operation on the IPduGroupVector must not trigger any change to the IPduGroupReInitVector.
     * Any clear or set operation performed on the IPduGroupReInitVector must trigger the same operation
     * on the IPduGroupVector.
     * These rules are needed in order to ensure consistency of the data sent to Com.
     */
    if (TRUE == action->RequestInit)
    {
      inst->RunTimeContext.IPduGroupReInitVector[i] |= action->EnableMask[i];
      inst->RunTimeContext.IPduGroupReInitVector[i] &= action->DisableMask[i];
    }
    else
    {
    /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule3,1 */
    /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule4,1 */
      inst->RunTimeContext.IPduGroupReInitVector[i] &= action->DisableMask[i];
    }

    /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule1,1 */
    inst->RunTimeContext.IPduGroupVector[i] |= action->EnableMask[i];
    /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule2,1 */
    inst->RunTimeContext.IPduGroupVector[i] &= action->DisableMask[i];

#if (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON)
     /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule6,1 */
     inst->RunTimeContext.DmcIPduGroupVector[i] |= action->EnableMask[i];
     /* !LINKSTO BswM.Impl.ComIpduGroupConsistency.Rule7,1 */
     inst->RunTimeContext.DmcIPduGroupVector[i] &= action->DisableMask[i];
#endif
  }
}
#endif

#if (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON)
FUNC(void, BSWM_CODE) BswM_ExecuteDeadLineMonitoringControlAction
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  BswMIndexType pbIndex
)
{
  BswM_IndexType i;

  inst->RunTimeContext.DeadlineMonitoringControlTriggered = TRUE;

  for (i = 0U; i < BSWM_IPDU_GROUP_VECTOR_SIZE; i++)
  {
    inst->RunTimeContext.DmcIPduGroupVector[i] |=
      BSWM_GET_DEADLINE_MONITORING_CONTROL_ACTION_TABLE(inst->ID)[pbIndex].EnableMask[i];
    inst->RunTimeContext.DmcIPduGroupVector[i] &=
      BSWM_GET_DEADLINE_MONITORING_CONTROL_ACTION_TABLE(inst->ID)[pbIndex].DisableMask[i];
  }
}
#endif

#if (BSWM_PDU_ROUTER_CONTROL_ACTION_USED == STD_ON)
FUNC(void, BSWM_CODE) BswM_ExecutePduRouterControlAction
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  BswMIndexType pbIndex
)
{
  /* !LINKSTO BswM.Impl.RunTime.PartitionContext,1 */
  BswMPduRouterControlActionType action = BSWM_GET_PDU_ROUTER_CONTROL_ACTION_TABLE(inst->ID)[pbIndex];
  BswM_IndexType i;

  for (i = action.Range.Head; i < action.Range.Tail; i++)
  {
    if (BSWM_PDUR_ENABLE == action.ActionType)
    {
      PduR_EnableRouting(BSWM_GET_PDUR_ROUTING_PATH_GROUP_ID_TABLE(inst->ID)[i]);
    }
    else
    {
      PduR_DisableRouting(BSWM_GET_PDUR_ROUTING_PATH_GROUP_ID_TABLE(inst->ID)[i]);
    }
  }
}
#endif

/*==================[internal function definitions]=========================*/

STATIC FUNC(BswMIndexType, BSWM_CODE) BswM_ExecuteRule
(
  P2CONST(BswMRuleType, AUTOMATIC, BSWM_CONST) rulePtr,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
)
{
  /* The logical expression result. */
  BswMResultType exprResult;
  /* The index of the action list to execute during the mode control phase. */
  BswMIndexType actionListIndex = BSWM_ARTIFACT_UNDEFINED;

  /* If one of the associated mode request ports are in an undefined state, the
   * logical expression shall return BSWM_UNDEFINED.  If BSWM_UNDEFINED is
   * returned the rule's state shall not be updated. */
  exprResult = BswM_EvaluateLogicalExpression(inst, rulePtr->LogicalExprIndex);

  /* A true or false action list is optional.  Therefore, before accessing the
   * action list, a check must be made to verify if the action list has been
   * defined. */
  if (BSWM_TRUE == exprResult)
  {
    if (rulePtr->TrueActionListIndex != BSWM_ARTIFACT_UNDEFINED)
    {
      /* If the associated action list is triggered, the value of the expression
       * must have changed. */
      boolean isTriggered = BSWM_GET_ACTION_LIST_TABLE(inst->ID)[rulePtr->TrueActionListIndex].IsTriggered;
      if (FALSE == isTriggered)
      {
        actionListIndex = rulePtr->TrueActionListIndex;
      }
      else if(exprResult != inst->RunTimeContext.RuleResultTable[rulePtr->ID])
      {
        actionListIndex = rulePtr->TrueActionListIndex;
      }
      else
      {
        /* Rule result has not changed. */
      }
    }
    inst->RunTimeContext.RuleResultTable[rulePtr->ID] = exprResult;
  }
  else if (BSWM_FALSE == exprResult)
  {
    if (rulePtr->FalseActionListIndex != BSWM_ARTIFACT_UNDEFINED)
    {
      boolean isTriggered = BSWM_GET_ACTION_LIST_TABLE(inst->ID)[rulePtr->FalseActionListIndex].IsTriggered;
      if (FALSE == isTriggered)
      {
        actionListIndex = rulePtr->FalseActionListIndex;
      }
      else if (exprResult != inst->RunTimeContext.RuleResultTable[rulePtr->ID])
      {
        actionListIndex = rulePtr->FalseActionListIndex;
      }
      else
      {
        /* Rule result has not changed. */
      }
    }
    inst->RunTimeContext.RuleResultTable[rulePtr->ID] = exprResult;
  }
  else
  {
    /* Ignore rule evaluation */
  }

  /* If no action list shall be executed, BSWM_ARTIFACT_UNDEFINED shall be
   * returned. */
  return actionListIndex;
}

STATIC FUNC(void, BSWM_CODE) BswM_PushActionList
(
  P2CONST(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(BswMActionListContextStackType, AUTOMATIC, AUTOMATIC) stackPtr,
  P2CONST(BswMActionListType, AUTOMATIC, BSWM_CONST) actionListPtr
)
{
#if (BSWM_PRECONDITION_ASSERT_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(inst);
#endif
  BSWM_PRECONDITION_ASSERT(inst->ID, (stackPtr->StackIndex < BSWM_ACTION_LIST_STACK_MAX),
    BSWM_INTERNAL_API_ID);
  stackPtr->StackIndex++;
  stackPtr->StackElements[stackPtr->StackIndex].NextItemIndex =
    actionListPtr->ActionListItemSegment.Head;
  stackPtr->StackElements[stackPtr->StackIndex].TailItemIndex =
    actionListPtr->ActionListItemSegment.Tail;
}

STATIC FUNC(void, BSWM_CODE) BswM_PushActionListContext
(
  P2CONST(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(BswMActionListContextStackType, AUTOMATIC, AUTOMATIC) stackPtr,
  BswMActionListContextType element
)
{
#if (BSWM_PRECONDITION_ASSERT_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(inst);
#endif
  BSWM_PRECONDITION_ASSERT(inst->ID, (stackPtr->StackIndex < BSWM_ACTION_LIST_STACK_MAX),
    BSWM_INTERNAL_API_ID);
  stackPtr->StackIndex++;
  stackPtr->StackElements[stackPtr->StackIndex] = element;
}

STATIC FUNC(BswMActionListContextType, BSWM_CODE) BswM_PopActionListContext
(
  P2VAR(BswMActionListContextStackType, AUTOMATIC, BSWM_VAR) stackPtr
)
{
  BswMActionListContextType topContext = stackPtr->StackElements[stackPtr->StackIndex];
  stackPtr->StackIndex--;
  return topContext;
}

STATIC FUNC(void, BSWM_CODE) BswM_ExecuteActionList
(
  P2CONST(BswMActionListType, AUTOMATIC, BSWM_CONST) actionListPtr,
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
  ,P2VAR(BswM_DeferredActionsQueueType, AUTOMATIC, AUTOMATIC) deferredActionsQueue
#endif
)
{
  Std_ReturnType result = E_OK;
  /* The current action list item to execute. */
  P2CONST(BswMActionListItemType, AUTOMATIC, BSWM_CONST) currentItemPtr;
  /* The action list execution stack. The uppermost stack element shall always
   * be processed first. */
  BswMActionListContextStackType stack = BSWM_NEW_ACTION_LIST_CONTEXT_STACK;

  BswM_PushActionList(inst, &stack, actionListPtr);

  while (FALSE == BSWM_STACK_EMPTY(stack))
  {
    boolean isInterrupted = FALSE;
    BswMActionListContextType actionListContext = BswM_PopActionListContext(&stack);

    while
    (
      (FALSE == isInterrupted) &&
      (actionListContext.NextItemIndex < actionListContext.TailItemIndex)
    )
    {
      currentItemPtr = &BSWM_GET_ACTION_LIST_ITEM_TABLE(inst->ID)[actionListContext.NextItemIndex];
      actionListContext.NextItemIndex++;

      switch(currentItemPtr->ActionListItemType)
      {
        case BSWM_ACTION:
        {
#if ( (BSWM_PDU_GROUP_SWITCH_ACTION_USED == STD_ON) ||         \
      (BSWM_DEADLINE_MONITORING_CONTROL_ACTION_USED == STD_ON) )
          if ( (currentItemPtr->ActionType == BSWM_PDU_GROUP_SWITCH_ACTION) ||
               (currentItemPtr->ActionType == BSWM_DEADLINE_MONITORING_CONTROL_ACTION)
             )
          {
            deferredActionsQueue->data[deferredActionsQueue->size] = currentItemPtr->ArtifactIndex;
            deferredActionsQueue->size++;
            result = E_OK;
          }
          else
#endif
          {
            result = BSWM_EXECUTE_ACTION(currentItemPtr->ArtifactIndex);
          }

#if ( BSWM_DEM_ENABLED == STD_ON)
          if ( currentItemPtr->DemEventId > 0)
          {
            /* !LINKSTO BswM0933_Conf,3 */
            Dem_EventStatusType demStatus = (result == E_OK) ?
                                            DEM_EVENT_STATUS_PASSED : DEM_EVENT_STATUS_FAILED;
            /* !LINKSTO BswM0007,2 */
            Dem_ReportErrorStatus(currentItemPtr->DemEventId, demStatus);
          }
#endif

          break;
        }
        case BSWM_ACTION_LIST:
        {
          /* Interrupt processing of the current action list, and begin
           * processing the nested action list. After the nested action list
           * completes, continue processing the original action list. */
          BswM_PushActionListContext(inst, &stack, actionListContext);
          BswM_PushActionList(inst, &stack, &BSWM_GET_ACTION_LIST_TABLE(inst->ID)[currentItemPtr->ArtifactIndex]);
          isInterrupted = TRUE;
          break;
        }
        case BSWM_RULE:
        {
          BswMIndexType actionListIndex;
          inst->SchMEnter();
          actionListIndex = BswM_ExecuteRule(&BSWM_GET_RULE_TABLE(inst->ID)[currentItemPtr->ArtifactIndex], inst);
          inst->SchMExit();
          if (actionListIndex != BSWM_ARTIFACT_UNDEFINED)
          {
            /* Interrupt processing of the current action list, and begin
             * processing the nested action list. After the nested action list
             * completes, continue processing the original action list. */
            BswM_PushActionListContext(inst, &stack, actionListContext);
            BswM_PushActionList(inst, &stack, &BSWM_GET_ACTION_LIST_TABLE(inst->ID)[actionListIndex]);
            isInterrupted = TRUE;
          }
          break;
        }
        /* CHECK: NOPARSE */
        default:
        {
          /* Shall never be entered. */
          BSWM_UNREACHABLE_CODE_ASSERT(inst->ID, BSWM_INTERNAL_API_ID);
          break;
        }
        /* CHECK: PARSE */
      }

      /* !LINKSTO BswM0055,1 */
      if ((TRUE == currentItemPtr->IsAbortOnFail) && (E_OK != result))
      {
        isInterrupted = TRUE;
      }
    }
  }
}

FUNC(void, BSWM_CODE) BswM_HandleRequest
(
  BswM_PartitionContextType* inst,
  void* port,
  uint32 mode,
  uint8 type,
  uint8 sid,
  boolean isImmediate
)
{
/* !LINKSTO BswM.Impl.BswMQueuedArbitration.Enable,1 */
#if (BSWM_QUEUED_ARBITRATION_ENABLED == STD_ON)
/* !LINKSTO BswM.Impl.BswMQueuedArbitration.Immediate.Deferred,1 */
#if (BSWM_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(sid);
#endif
#if (BSWM_QUEUE_DEFERRED_REQUESTS == STD_OFF)
  if (isImmediate == FALSE)
  {
    BswM_ExecuteModeArbitration(inst, port, mode, type);
  }
  else
#endif
  {
    TS_PARAM_UNUSED(isImmediate);
    inst->SchMEnter();
    if (inst->RunTimeContext.BswMRequestQueue.active == FALSE)
    {
      BswMRequestType* request;
      inst->RunTimeContext.BswMRequestQueue.active = TRUE;
      inst->SchMExit();

      BswM_ExecuteModeArbitration(inst, port, mode, type);

      /* !LINKSTO BswM.Impl.BswMQueuedArbitration.Dequeue.Immediate,1 */
      inst->SchMEnter();
      while((request = BswM_DequeueModeRequest(&(inst->RunTimeContext.BswMRequestQueue))) != NULL_PTR)
      {
        inst->SchMExit();
        BswM_ExecuteModeArbitration(inst, request->port, request->mode, request->type);
        inst->SchMEnter();
      }

      inst->RunTimeContext.BswMRequestQueue.active = FALSE;
#if (BSWM_DELAY_MAINFUNCTION == STD_ON)
      if (inst->RunTimeContext.MainFunctionDelayed == TRUE)
      {
        inst->RunTimeContext.MainFunctionDelayed = FALSE;
        inst->SchMExit();
        /* !LINKSTO BswM.Impl.BswMQueuedArbitration.Dequeue.Immediate.MainFunctionDelay,1 */
        BswM_MainFunction();
      }
      else
#endif
      {
        inst->SchMExit();
      }
    }
    /* !LINKSTO BswM.Impl.BswMQueuedArbitration.Immediate.Immediate,1 */
    /* !LINKSTO BswM.Impl.BswMQueuedArbitration.Immediate.Deferred,1 */
    /* !LINKSTO BswM.Impl.BswMQueuedArbitration.MainFunction.Immediate, 1*/
    /* !LINKSTO BswM.Impl.BswMQueuedArbitration.MainFunction.Deferred, 1*/
    else
    {
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
      Std_ReturnType ret = BswM_EnqueueModeRequest(inst, port, mode, type);
#else
      (void) BswM_EnqueueModeRequest(inst, port, mode, type);
#endif
      inst->SchMExit();
#if (BSWM_DEV_ERROR_DETECT == STD_ON)
      if ((ret == E_NOT_OK) && (sid != BSWM_SID_INVALID))
      {
        /* !LINKSTO BswM.Impl.BswMQueuedArbitration.QueueOverflow,1 */
        BSWM_DET_REPORT_ERROR(inst->ID, sid, BSWM_E_QUEUE_FULL);
      }
#endif
    }
  }
#else
  TS_PARAM_UNUSED(isImmediate);
  TS_PARAM_UNUSED(sid);
  BswM_ExecuteModeArbitration(inst, port, mode, type);
#endif
}

#if (BSWM_QUEUED_ARBITRATION_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, BSWM_CODE) BswM_EnqueueModeRequest
(
  P2VAR(BswM_PartitionContextType, AUTOMATIC, BSWM_VAR) inst,
  P2VAR(void, AUTOMATIC, BSWM_VAR) port,
  uint32 mode,
  uint8 type
)
{
  P2VAR(BswMRequestQueueType, AUTOMATIC, BSWM_VAR) queue = &(inst->RunTimeContext.BswMRequestQueue);
  Std_ReturnType ret = E_NOT_OK;
  if (queue->size < BSWM_REQUEST_QUEUE_MAX)
  {
    queue->rear = (queue->rear + 1) % BSWM_REQUEST_QUEUE_MAX;
    queue->elements[queue->rear].type = type;
    queue->elements[queue->rear].mode = mode;
    queue->elements[queue->rear].port = port;
    queue->size++;
    ret = E_OK;
  }
  else
  {
    ret = E_NOT_OK;
  }
  return ret;
}

STATIC FUNC_P2VAR(BswMRequestType, BSWM_VAR, BSWM_CODE) BswM_DequeueModeRequest
(
  P2VAR(BswMRequestQueueType, AUTOMATIC, BSWM_VAR) queue
)
{
  P2VAR(BswMRequestType, AUTOMATIC, BSWM_VAR) ret = NULL_PTR;
  if (queue->size > 0)
  {
    ret = &(queue->elements[queue->front]);
    queue->front = (queue->front + 1) % BSWM_REQUEST_QUEUE_MAX;
    queue->size--;
  }
  return ret;
}

#endif

#define BSWM_STOP_SEC_CODE
#include <BswM_MemMap.h>

/*==================[end of file]===========================================*/
