#ifndef SD_SEND_INT_H_
#define SD_SEND_INT_H_

/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Sd_Int.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>


/** \brief initialize SW-Unit internal state variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_init(void);

/** \brief initialize and prepare transmission buffer
 **
 ** \param[in] instanceIdx  index of the Sd instance to transmit on
 ** \param[in] txStatePtr  remote node state variable to transmit to
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_setupBuffer
(
  CONSTP2VAR(Sd_TxStateType,AUTOMATIC,SD_APPL_DATA) txStatePtr,
  uint8 instanceIdx
);

/** \brief perform transmission of prepared transmission buffer
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_flushBuffer(void);


/** \brief Put a FindService entry into the transmission buffer
 **
 ** \param[in] serviceIdx  service index
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueFindService
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr,
  uint16 serviceIdx
);

/** \brief Put a OfferService entry into the transmission buffer
 **
 ** \param[in] serviceIdx  service index
 ** \param[in] isStopOffer          TRUE: transmit a StopOffer, FALSE: transmit a Offer
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueOfferService
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr,
  const uint16 serviceIdx,
  const boolean isStopOffer
);

/** \brief Put a SubscribeEventGroup entry into the transmission buffer
 **
 ** \param[in] eventGroupIdx  event group index
 ** \param[in] isStopSubscribe         TRUE: UnsubscribeEventGroup, FALSE: SubscribeEventGroup
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroup
(
  const uint16 eventGroupIdx,
  const SoAd_SoConIdType UniUdpSoConId,
  const SoAd_SoConIdType UniTcpSoConId,
  const boolean isStopSubscribe
);


/** \brief Put a SubscribeEventGroupAck entry into the transmission buffer
 **
 ** \param[in] TTL            time to live
 ** \param[in] serviceIdx     service index
 ** \param[in] eventGroupIdx  event group index
 ** \param[in] reservedField  Value of reserved field
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupAck
(
  const uint32 TTL,
  const uint16 serviceIdx,
  const uint16 eventGroupIdx,
  const uint16 reservedField
);

/** \brief Put a SubscribeEventGroupNack entry for a configured service
 **        into the transmission buffer
 **
 ** \param[in] serviceIdx     service index
 ** \param[in] reservedField  Value of reserved field
 ** \param[in] eventGroupID   event group Id
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupNack
(
  const uint16 serviceIdx,
  const uint16 reservedField,
  const uint16 eventGroupId
);

/** \brief Put a SubscribeEventGroupNack entry for a unknown service
 **        into the transmission buffer
 **
 ** \param[in] serviceID      Service Id
 ** \param[in] instanceID     Instance Id
 ** \param[in] eventGroupId   EventGroup Id
 ** \param[in] reservedField  Value of reserved field
 ** \param[in] majorVersion   Major Version
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupNackUnknown
(
  uint16 serviceID,
  uint16 instanceId,
  uint16 eventGroupId,
  uint16 reservedField,
  uint8 majorVersion
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* SD_SEND_INT_H_ */
