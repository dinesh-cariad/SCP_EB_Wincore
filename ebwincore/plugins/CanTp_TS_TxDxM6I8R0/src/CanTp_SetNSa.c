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

/** \brief CanTp_SetNSa()
 **
 ** This API-Service is used to set a N_SA value for a specific PDU.
 */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_SetNSa
(
  PduIdType CanTpPduId,
  uint8     CanTpDirection,
  uint8     CanTpNSa
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  Std_ReturnType  Ret = E_NOT_OK; /* return value */

  DBG_CANTP_SETNSA_ENTRY(CanTpPduId, CanTpDirection, CanTpNSa);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  /* check CanTp state - possible DET and return on error */
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_SETNSA,
                            CANTP_E_UNINIT
                          );
  }
  /* check for correct parameter for direction - possible DET and return on error */
  else if(
           (CANTP_PDU_DIR_RECEIVE != CanTpDirection)
           && (CANTP_PDU_DIR_TRANSMIT != CanTpDirection)
         )
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_SETNSA,
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
                            CANTP_API_SETNSA,
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
                            CANTP_API_SETNSA,
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
      TS_AtomicAssign8(CanTp_NSaValues[NSaIndex], CanTpNSa);
    }
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    else
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
                              CANTP_API_SETNSA,
                              ErrorId
                            );
    }
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  }

  DBG_CANTP_SETNSA_EXIT(Ret, CanTpPduId, CanTpDirection, CanTpNSa);
  return Ret;
} /* CanTp_SetNSa() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{
  Std_ReturnType  Ret = E_NOT_OK;
  DBG_CANTP_SETNSA_ENTRY(CanTpPduId, CanTpDirection, CanTpNSa);
  Ret = (CanTp_EntryTablePtr->SetNSa)(CanTpPduId, CanTpDirection, CanTpNSa);
  DBG_CANTP_SETNSA_EXIT(Ret, CanTpPduId, CanTpDirection, CanTpNSa);
  return Ret;
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetNSaIndex
(
  PduIdType NSduId,
  uint8 Direction,
  P2VAR(CanTp_RxNPduLookupEntryType, AUTOMATIC, CANTP_VAR) NSaIndexPtr
)
{
  Std_ReturnType  Ret = E_NOT_OK;
  uint8 AddressingFormat;
  PduIdType RxNPduId;

  if(Direction == CANTP_PDU_DIR_RECEIVE)
  {
    AddressingFormat = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).AddressingFormat;
    RxNPduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).NPduIdxF;
    Ret = E_OK;
  }
  else /* must be CANTP_PDU_DIR_TRANSMIT */
  {
    /* Only for physical target address type FC frames will be received.
       N_SA can only be changed  for physical target address. */
    if(CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).Tatype == CANTP_PHYSICAL)
    {
      AddressingFormat = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).AddressingFormat;
      RxNPduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).NPduIdFc;
      Ret = E_OK;
    }
    else
    {
      AddressingFormat = CANTP_STANDARD;
      RxNPduId = 0U;
      Ret = E_NOT_OK;
    }
  }

  /* N_SA can only be changed for extended addressing */
  if(CANTP_EXTENDED != AddressingFormat)
  {
    Ret = E_NOT_OK;
  }

  if(E_OK == Ret)
  {
    CanTp_RxNPduLookupEntryType NSaIndex;
    P2CONST(CanTp_RxNPduTableType, AUTOMATIC, CANTP_APPL_CONST) RxNPduTableEntryPtr;
    CanTp_RxNPduLookupEntryType NextIndex  = CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(RxNPduId);
    Ret = E_NOT_OK;

    do
    {
      RxNPduTableEntryPtr = &CANTP_CFG_GET_RXNPDU_TABLE(NextIndex);

      NSaIndex = NextIndex; /* Current index */

      NextIndex = RxNPduTableEntryPtr->Next; /* Prepare next index */

      if(RxNPduTableEntryPtr->Direction == Direction)
      {
        if(RxNPduTableEntryPtr->Index == NSduId)
        {
          /* This NSaIndex fits to the NSduId. Search finished. */
          Ret = E_OK;
          *NSaIndexPtr = NSaIndex; /* return index */
          NextIndex = 0U; /* Leave loop */
        }
      }
    } while(NextIndex != 0); /* Index == 0 means no further table entries for this N-PDU */
  }

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
