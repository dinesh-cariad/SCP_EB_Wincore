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
#include <TSAtomic_Assign.h>   /* Header for atomic assignment macros */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Api.h>         /* Module public API */
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

#if(CANTP_DYNAMIC_NSA_API == STD_ON)
/** \brief CanTp_GetNSa()
 **
 ** This API-Service is used to get a N_SA value for a specific PDU.
*/
FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetNSa
(
  PduIdType CanTpPduId,
  uint8 CanTpDirection,
  P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) CanTpNSaPtr
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType  Ret = E_NOT_OK; /* return value */

  DBG_CANTP_GETNSA_ENTRY(CanTpPduId, CanTpDirection, CanTpNSaPtr);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)

  /* check CanTp state - possible DET and return on error */
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETNSA,
                            CANTP_E_UNINIT
                          );
  }

  /* check for Null pointer - possible DET and return on error */
  /* if pointer is null */
  else if(CanTpNSaPtr == NULL_PTR)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETNSA,
                            CANTP_E_PARAM_POINTER
                          );
  }

  /* check for direction parameter - possible DET and return on error */
  else if(
           (CANTP_PDU_DIR_RECEIVE != CanTpDirection)
           && (CANTP_PDU_DIR_TRANSMIT != CanTpDirection)
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETNSA,
                            CANTP_E_PARAM_DIRECTION
                          );
  }
  /* validate CanTpPduId == NSduId for Rx Sdus*/
  else if(
           (CANTP_PDU_DIR_RECEIVE  == CanTpDirection)
           && (CanTpPduId >= CanTp_CfgPtr->NumberRxNSdus)
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETNSA,
                            CANTP_E_INVALID_RX_ID
                          );
  }
  /* validate CanTpPduId == NSduId for Tx Sdus*/
  else if(
           (CANTP_PDU_DIR_TRANSMIT == CanTpDirection)
           && (CanTpPduId >= CanTp_CfgPtr->NumberTxNSdus)
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_GETNSA,
                            CANTP_E_INVALID_TX_ID
                          );
  }
  else
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  {
    CanTp_RxNPduLookupEntryType NSaIndex = 0U;

    Ret = CanTp_GetNSaIndex(CanTpPduId, CanTpDirection, &NSaIndex);

    if(E_OK == Ret)
    {
      TS_AtomicAssign8(*CanTpNSaPtr, CanTp_NSaValues[NSaIndex]);
    }

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    if(E_OK != Ret)
    {
      uint8 ErrorId;
      /* no N-PDU found */
      if(CANTP_PDU_DIR_RECEIVE == CanTpDirection)
      {
        ErrorId = CANTP_E_INVALID_RX_ID;
      }
      else /* must be CANTP_PDU_DIR_TRANSMIT */
      {
        ErrorId = CANTP_E_INVALID_TX_ID;
      }

      /* call DET with error CANTP_E_INVALID_TX_ID or CANTP_E_INVALID_RX_ID*/
      CANTP_DET_REPORT_ERROR(
                              CANTP_MODULE_ID,
                              CANTP_INSTANCE_ID,
                              CANTP_API_GETNSA,
                              ErrorId
                            );
    }
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  }

  DBG_CANTP_GETNSA_EXIT(Ret, CanTpPduId, CanTpDirection, CanTpNSaPtr);
  return Ret;
} /* CanTp_GetNSa() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType Ret = E_NOT_OK;
  DBG_CANTP_GETNSA_ENTRY(CanTpPduId, CanTpDirection, CanTpNSaPtr);
  Ret = (CanTp_EntryTablePtr->GetNSa)(CanTpPduId, CanTpDirection, CanTpNSaPtr);
  DBG_CANTP_GETNSA_EXIT(Ret, CanTpPduId, CanTpDirection, CanTpNSaPtr);
  return Ret;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_DYNAMIC_NSA_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

/*==================[internal function definitions]==========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]============================================*/
