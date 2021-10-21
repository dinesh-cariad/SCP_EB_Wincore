#ifndef SD_TYPES_H
#define SD_TYPES_H

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

#include <ComStack_Types.h>         /* AUTOSAR standard types */
#include <Sd_Cfg.h>

/** \brief Enable/disable relocateable config */
#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE    SD_RELOCATABLE_CFG_ENABLE

#if (defined TS_PB_CFG_PTR_CLASS)
#error TS_PB_CFG_PTR_CLASS is already defined
#endif
#define TS_PB_CFG_PTR_CLASS          SD_CONST

#include <TSPBConfig_Types.h>


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* !LINKSTO Sd.ASR40.SWS_SD_00551,1 */
/**
 ** \brief This type defines the client service states that are reported to the BswM
 **        using the expected API BswM_Sd_ClientServiceCurrentState().
 **/
#define SD_CLIENT_SERVICE_DOWN       0U
#define SD_CLIENT_SERVICE_AVAILABLE  1U
typedef uint8 Sd_ClientServiceCurrentStateType;

/* !LINKSTO Sd.ASR40.SWS_SD_00552,1 */
/**
 ** \brief This type defines the consumed eventgroup states that are reported to the
 **        BswM using the expected API BswM_Sd_ConsumedEventGroupCurrentState().
 **/
#define SD_CONSUMED_EVENTGROUP_DOWN      0U
#define SD_CONSUMED_EVENTGROUP_AVAILABLE 1U
typedef uint8 Sd_ConsumedEventGroupCurrentStateType;

/* !LINKSTO Sd.ASR40.SWS_SD_00553,1 */
/**
 ** \brief This type defines the event handler states that are reported to the
 **        BswM using the expected API BswM_Sd_EventHandlerCurrentState().
 **/
#define SD_EVENT_HANDLER_RELEASED    0U
#define SD_EVENT_HANDLER_REQUESTED   1U
typedef uint8 Sd_EventHandlerCurrentStateType;

/* !LINKSTO Sd.ASR40.SWS_SD_00118,1 */
/**
 ** \brief This type defines the Server service states that are reported to the SD
 **        using the expected API Sd_ServerServiceSetState().
 **/
#define SD_SERVER_SERVICE_DOWN       0U
#define SD_SERVER_SERVICE_AVAILABLE  1U
typedef uint8 Sd_ServerServiceSetStateType;

/* !LINKSTO Sd.ASR40.SWS_SD_00405,1 */
/**
 ** \brief This type defines the Client service states that are reported to the BswM
 **        using the expected API Sd_ClientServiceSetState().
 **/
#define SD_CLIENT_SERVICE_RELEASED   0U
#define SD_CLIENT_SERVICE_REQUESTED  1U
typedef uint8 Sd_ClientServiceSetStateType;

/* !LINKSTO Sd.ASR40.SWS_SD_00550,1 */
/**
 ** \brief This type defines the consumed eventgroup states that are reported to the SD
 **        using the expected API Sd_ConsumedEventGroupSetState().
 **/
#define SD_CONSUMED_EVENTGROUP_RELEASED  0U
#define SD_CONSUMED_EVENTGROUP_REQUESTED 1U
typedef uint8 Sd_ConsumedEventGroupSetStateType;

#if(SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON)
typedef P2FUNC (void, TYPEDEF, Sd_Server_Not_Available_FpType)
(
  uint16 ServiceID,
  uint16 InstanceID,
  TcpIp_SockAddrType* UdpIpAddrPtr,
  TcpIp_SockAddrType* TcpIpAddrPtr
);
#endif /* SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON */

#if(SD_MALFORMED_MSG_CALLBACK == STD_ON)
typedef P2FUNC (void, TYPEDEF, Sd_User_Malformed_Msg_FpType)
(
  void
);
#endif /* SD_MALFORMED_MSG_CALLBACK == STD_ON */

#if(SD_SUBSCR_NACK_RECV_CALLBACK == STD_ON)
typedef P2FUNC (void, TYPEDEF, Sd_User_SubscribeNackReceived_FpType)
(
  void
);
#endif /* SD_SUBSCR_NACK_RECV_CALLBACK == STD_ON */

/*==================[external function declarations]========================*/
#if(SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON)
extern CONST(Sd_Server_Not_Available_FpType, SD_APPL_CONST) Sd_User_Server_Not_Available_Fp;
#endif /* SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON */

#if(SD_MALFORMED_MSG_CALLBACK == STD_ON)
extern CONST(Sd_User_Malformed_Msg_FpType, SD_APPL_CONST) Sd_User_Malformed_Msg_Fp;
#endif /* SD_MALFORMED_MSG_CALLBACK == STD_ON */

#if(SD_SUBSCR_NACK_RECV_CALLBACK == STD_ON)
extern CONST(Sd_User_SubscribeNackReceived_FpType, SD_APPL_CONST) Sd_User_SubscribeNackReceived_Fp;
#endif /* SD_SUBSCR_NACK_RECV_CALLBACK == STD_ON */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#undef TS_RELOCATABLE_CFG_ENABLE

#undef TS_PB_CFG_PTR_CLASS

#endif /* ifndef SD_TYPES_H */
/*==================[end of file]===========================================*/
