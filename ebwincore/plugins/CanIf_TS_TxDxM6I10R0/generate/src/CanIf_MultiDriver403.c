/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

/*==================[inclusions]============================================*/
[!IF "CanIfPublicCfg/CanIfPublicMultipleDrvSupport = 'true' and (CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR402' or CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR403')"!]

[!INCLUDE "../../generate_macros/CanIf_PreCompileMacros.m"!]
[!INCLUDE "../../generate_macros/CanIf_Macros.m"!]
#include <CanIf_Can.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>
  [!FOR "i"="0" TO "$canDriverNum - 1"!]
    [!VAR "CanDriverInfix"!][!CALL "CreateCanDriverInfixByIdx", "idx"="$i"!][!ENDVAR!]

/** \brief Transmit confirmation callback function derivate for driver [!"$CanDriverInfix"!] */
FUNC( void, CANIF_CODE ) CanIf_TxConfirmation_[!"$CanDriverInfix"!]( PduIdType CanTxPduId )
{
  /* Call the original CanIf_TxConfirmation */
  CanIf_TxConfirmation(CanTxPduId);
}


  [!VAR "HohOffset"!][!CALL "GetDriverHohOffset", "idx"="$i"!][!ENDVAR!]
/** \brief Receive indication callback function derivate for driver [!"$CanDriverInfix"!] */
FUNC( void, CANIF_CODE ) CanIf_RxIndication_[!"$CanDriverInfix"!]
  (
    Can_HwHandleType Hrh,
    Can_IdType CanId,
    uint8 CanDlc,
    P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtr
  )
{
  
    [!IF "$i = '0'"!]
  /* Call the original CanIf_Rxindication */
  CanIf_RxIndication(Hrh, CanId, CanDlc, CanSduPtr);
    [!ELSE!]
  /* Call the original CanIf_Rxindication with the offset applied to HRH id */
  CanIf_RxIndication(Hrh + [!"$HohOffset"!]u, CanId, CanDlc, CanSduPtr);
    [!ENDIF!]
}

    [!IF "$canifCancelTxConfSupported = 'true'"!]
/** \brief Transmit cancellation confirmation callback function derivate for driver [!"$CanDriverInfix"!] */
FUNC( void, CANIF_CODE ) CanIf_CancelTxConfirmation_[!"$CanDriverInfix"!]
(
      [!IF "$canDriverCompatibility != 'ASR402'"!]
  PduIdType CanTxPduId,
  P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
      [!ELSE!]
  P2CONST( Can_PduType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
      [!ENDIF!][!/* $canDriverCompatibility != 'ASR402' */!]
)
{
  /* Call the original CanIf_CancelTxConfirmation */
      [!IF "$canDriverCompatibility != 'ASR402'"!]
  CanIf_CancelTxConfirmation(CanTxPduId, PduInfoPtr);
      [!ELSE!]
  CanIf_CancelTxConfirmation(PduInfoPtr);
      [!ENDIF!][!/* $canDriverCompatibility != 'ASR402' */!]
}
    [!ENDIF!][!/* canifCancelTxConfSupported */!]

/** \brief Bus-off callback function derivate for driver [!"$CanDriverInfix"!]  */
FUNC( void, CANIF_CODE ) CanIf_ControllerBusOff_[!"$CanDriverInfix"!]( uint8 Controller )
{
  /* Call the original CanIf_ControllerBusOff */
  CanIf_ControllerBusOff(Controller);
}

/** \brief Controller mode indication callback function derivate for driver [!"$CanDriverInfix"!] */
FUNC( void, CANIF_CODE ) CanIf_ControllerModeIndication_[!"$CanDriverInfix"!]
  (
    uint8 Controller,
    CanIf_ControllerModeType ControllerMode
  )
{
  /* Call the original CanIf_ControllerModeIndication */
  CanIf_ControllerModeIndication(Controller, ControllerMode);
}
  [!ENDFOR!]


#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>
[!ELSE!]
#include <TSCompiler.h>

TS_PREVENTEMPTYTRANSLATIONUNIT

[!ENDIF!][!//CanIfPublicMultipleDrvSupport = 'true' and (CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR402' or CanIfPublicCfg/CanIfCanDriverCompatibility = 'ASR403')
/*==================[end of file]===========================================*/
[!ENDCODE!][!//
