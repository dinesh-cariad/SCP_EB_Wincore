/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ECUC_TYPES_H
#define ECUC_TYPES_H

/*==================[inclusions]============================================*/
#include <TSAutosar.h>            /* basic AUTOSAR types */
#include <ComStack_Types.h>       /* Com Stack Types */

/*==================[macros]================================================*/
/** \brief buffer is currently not used, no transmission ongoing */
#define ECUC_STATE_IDLE       0U
/** \brief TP reception is currently ongoing */
#define ECUC_STATE_RXONGOING  1U
/** \brief TP transmission is currently ongoing */
#define ECUC_STATE_TXONGOING  2U
/*==================[defensive programming macros]==========================*/

/*==================[type definitions]======================================*/
/** \brief Specifies the current state of the transmission */
typedef uint8 EcuC_StateType;

/** \brief Contains the control information of the receive buffer */
typedef struct
{
  PduLengthType remainMsgSize;
  /**< remaining data which need to be copied */
  PduLengthType totalMsgSize;
  /**< total size of the message which is copied */
  EcuC_StateType state;
  /**< current state of the reception */
} EcuC_RxControlInfoType;

/** \brief Pointer to an \a EcuC_RxControlInfoType */
typedef P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) EcuC_PtrRxControlInfoType;

/** \brief Contains the control information of the transmit buffer */
typedef struct
{
  /**< remaining data which need to be copied */
  PduLengthType remainMsgSize;
  /**< current state of the transmission */
  EcuC_StateType state;
}EcuC_TxControlInfoType;


/** \brief Contains the control information of the transmit function */
/**< Identification of the I-PDU */
/**< Length and pointer to the buffer of the I-PDU */
typedef P2FUNC(Std_ReturnType, ECUC_CODE, EcuC_LoTpTransmitFunctPtrType)
(
  PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_CONST) TxBufferInfoPtr
);

/** \brief Pointer to an \a EcuC_TxControlInfoType */
typedef P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) EcuC_PtrTxControlInfoType;


/*==================[external data declarations]============================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[end of file]===========================================*/
#endif /* ECUC_TYPES_H */
