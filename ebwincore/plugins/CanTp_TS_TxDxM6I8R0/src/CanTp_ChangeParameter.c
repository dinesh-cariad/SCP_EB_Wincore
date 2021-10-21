/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* ComStack types */
#include <CanTp_Api.h>         /* Module public API, config dependent */
#include <CanTp_Cfg.h>         /* CANTP_DEV_ERROR_DETECT, CANTP_CHANGE_PARAMETER_REQ_API */
#include <CanTp_Internal.h>    /* internal macros and variables */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/


#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */




/*==================[ChangeParam function]================*/
#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType ret_result = E_NOT_OK;

  DBG_CANTP_CHANGEPARAMETER_ENTRY(Id, Parameter, Value);
#if(CANTP_DEV_ERROR_DETECT == STD_ON)

  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGEPARAMETER,
                            CANTP_E_UNINIT
                          );
  }
  /* Check Id range - possible DET and return on error */
  else if(Id >= CanTp_CfgPtr->NumberRxNSdus)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGEPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else if((Parameter != TP_STMIN) && (Parameter != TP_BS))
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGEPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  /* !LINKSTO CanTp.ASR40.CANTP305_3, 1 */
  /* BS has a valid range of 0 - 255. STmin has a valid range of 0-127 and 241 - 249. */
  else if(((Parameter == TP_BS) && (Value > 255U))
          || ((Parameter == TP_STMIN)
              && ((Value > 249U) || ((Value > 127U) && (Value < 241U)))
             )
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGEPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  {
    CanTp_ChType  Channel;

    CANTP_ENTER_CRITICAL_SECTION();

    /* take the channel's index */
#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
    /* Don't change parameter when reception is in progress. */
    ret_result = CanTp_FindRxNsduChannelIndex(&Channel, Id);
#else
    {
      P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
      CanTp_ChStateType ChannelState;

      Channel = CANTP_CFG_GET_RXNSDU_CONFIG(Id).Channel;

      /* verify that configured Rx channel is free - get the channel pointer first */
      ChannelPtr = &CanTp_Channel[Channel];

      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
      {
        /* Check if the actual N-Sdu on the channel is the requested N-Sdu to be
          changed. */
        if(ChannelPtr->NSduId == Id)
        {
          /* !LINKSTO CanTp.ASR40.CANTP304_1, 1 */
          /* Don't change parameter when reception is in progress. */
          ret_result = E_NOT_OK;
        }
        else
        {
          /* Channel is in Rx mode for another N-Sdu. Continue. */
          ret_result = E_OK;
        }
      }
      else
      {
        /* Change parameter when no reception is in progress. */
        ret_result = E_OK;
      }
    }
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

    if(E_OK == ret_result)
    {
      /* All validation checks done.
         Now it is possible to change the requested parameter. */
      /* !LINKSTO CanTp.EB.MetaData.ChangeParam, 1 */
      if(Parameter == TP_BS)
      {
        /* Change Block size. Id = NSduId */
        CANTP_SET_RX_PDU_BLOCK_SIZE(Id, Value);
      }
      else
      {
        /* Change separation time. Id = NSduId */
        CANTP_SET_RX_PDU_ST_MIN(Id, Value);
      }
    }
    CANTP_LEAVE_CRITICAL_SECTION();
  }

  DBG_CANTP_CHANGEPARAMETER_EXIT(ret_result, Id, Parameter, Value);

  return ret_result;
}/* CanTp_ChangeParameter() */

#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType ret_result = E_NOT_OK;
  DBG_CANTP_CHANGEPARAMETER_ENTRY(Id, Parameter, Value);
  ret_result = (CanTp_EntryTablePtr->ChangeParameter)(Id, Parameter, Value);
  DBG_CANTP_CHANGEPARAMETER_EXIT(ret_result, Id, Parameter, Value);
  return ret_result;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

/*===================[End ChangeParam function]====================================================*/

/*==================[ChangeTxParam function]=======================================================*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeTxParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType ret_result = E_NOT_OK;

  DBG_CANTP_CHANGETXPARAMETER_ENTRY(Id, Parameter, Value);

  #if(CANTP_DEV_ERROR_DETECT == STD_ON)
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGETXPARAMETER,
                            CANTP_E_UNINIT
                          );
  }
  /* Check Id range - possible DET and return on error */
  else if(
               (Id >= CanTp_CfgPtr->NumberTxNSdus) ||
               (Parameter != TP_STMIN)
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGETXPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  /* BS has a valid range of 0 - 255. STmin has a valid range of 0-127 and 241 - 249. */
  else if(
               (Value > 249U) || ((Value > 127U) && (Value < 241U))
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_CHANGETXPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else
#else
  TS_PARAM_UNUSED(Parameter);
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Change separation time */
    TS_AtomicAssign8(CanTp_TxNSduStMinValues[Id], (uint8)Value);

    ret_result = E_OK;
  }

  DBG_CANTP_CHANGETXPARAMETER_EXIT(ret_result, Id, Parameter, Value);

  return ret_result;
}/* CanTp_ChangeTxParameter() */

#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType ret_result = E_NOT_OK;
  DBG_CANTP_CHANGETXPARAMETER_ENTRY(Id, Parameter, Value);
  ret_result = (CanTp_EntryTablePtr->ChangeTxParameter)(Id, Parameter, Value);
  DBG_CANTP_CHANGETXPARAMETER_EXIT(ret_result, Id, Parameter, Value);
  return ret_result;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */


/*==================[End ChangeTxParam function]=======================================================*/



/*==================[ChangeParameter function]==============*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeRxParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
)
{
  Std_ReturnType ret_result = CanTp_ChangeParameter(Id,Parameter,Value);

  return ret_result;
}

#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

/*=================[End ChangeParameter function]===========*/


/*==================[ResetTxParam function]=======================================================*/
#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)
#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
FUNC(void, CANTP_CODE) CanTp_ResetTxParameter
(
  PduIdType Id
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  DBG_CANTP_RESETTXPARAMETER_ENTRY(Id);
#if (CANTP_DEV_ERROR_DETECT == STD_ON)
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RESETTXPARAMETER,
                            CANTP_E_UNINIT
                          );
  }
  /* Check Id range - possible DET and return on error */
  else if(Id >= CanTp_CfgPtr->NumberTxNSdus)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RESETTXPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else
#endif
  {
    /* Set the value for TxParameter state */
    TS_AtomicAssign8(CanTp_TxNSduStMinValues[Id], 0xFFU);
  }
  DBG_CANTP_RESETTXPARAMETER_EXIT(Id);
}
#else /* CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  DBG_CANTP_RESETTXPARAMETER_ENTRY(Id);
  (CanTp_EntryTablePtr->ResetTxParameter)(Id);
  DBG_CANTP_RESETTXPARAMETER_EXIT(Id);
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */
#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */
#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */



#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */


/*==================[End of ResetTxParam function]=======================================================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
