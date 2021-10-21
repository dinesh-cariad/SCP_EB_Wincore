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
#include <CanTp_Cfg.h>         /* CANTP_DEV_ERROR_DETECT, CANTP_READ_PARAMETER_REQ_API */
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

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

#if(CANTP_READ_PARAMETER_REQ_API == STD_ON)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_ReadParameter
(
  PduIdType id,
  TPParameterType parameter,
  uint16* value
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType Ret = E_NOT_OK;

  DBG_CANTP_READPARAMETER_ENTRY(id, parameter, value);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)

  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_READPARAMETER,
                            CANTP_E_UNINIT
                          );
  }
  else if(id > CanTp_CfgPtr->NumberRxNSdus)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_READPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else if((parameter != TP_STMIN) && (parameter != TP_BS))
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_READPARAMETER,
                            CANTP_E_PARAM_ID
                          );
  }
  else if(value == NULL_PTR)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_READPARAMETER,
                            CANTP_E_PARAM_POINTER
                          );
  }
  else
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  {
    CANTP_ENTER_CRITICAL_SECTION();

    if(parameter == TP_STMIN)
    {
      *value = (uint16)CANTP_GET_RX_PDU_ST_MIN(id);
    }
    else /* Parameter == TP_BS */
    {
      *value = (uint16)CANTP_GET_RX_PDU_BLOCK_SIZE(id);
    }

    Ret = E_OK;

    CANTP_LEAVE_CRITICAL_SECTION();
  }

  DBG_CANTP_READPARAMETER_EXIT(Ret, id, parameter, value);

  return Ret;

}/* CanTp_ReadParameter() */

#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType ret_result = E_NOT_OK;
  DBG_CANTP_READPARAMETER_ENTRY(id, parameter, value);
  ret_result = (CanTp_EntryTablePtr->ReadParameter)(id, parameter, value);
  DBG_CANTP_READPARAMETER_EXIT(ret_result, id, parameter, value);
  return ret_result;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_READ_PARAMETER_REQ_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
