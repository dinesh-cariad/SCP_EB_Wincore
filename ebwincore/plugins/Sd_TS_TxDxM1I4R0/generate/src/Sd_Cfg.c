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

/*==================[includes]============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <Sd_Cfg.h>              /* Generated configuration */

/*==================[internal constants]=====================================*/

/*==================[inclusions]============================================*/
#include <Sd_Types.h>

[!LOOP "variant:all()"!][!//
#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_NAME
#undef TS_PB_CFG_LAYOUT_TYPE
#include "Sd_[!"."!]_PBcfg.c"

[!ENDLOOP!][!//

#if (!defined SD_NO_PBCFG_REQUIRED) && (!defined SD_NO_CFG_REQUIRED)
#if (defined SD_POSTBUILD_VARIANT_DISABLED)
#include "Sd_PBcfg.c"
#endif
#endif

/*==================[macros]=================================================*/
[!IF "not(node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackServerNotAvailable/Sd_ServerNotAvailableCallBack))"!]
CONST(Sd_Server_Not_Available_FpType, SD_APPL_CONST) Sd_User_Server_Not_Available_Fp =
    &[!"string(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackServerNotAvailable/Sd_ServerNotAvailableCallBack)"!];
[!ENDIF!]

[!IF "not(node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackMalformedMsg/Sd_MalformedMsgCallBack))"!]
CONST(Sd_User_Malformed_Msg_FpType, SD_APPL_CONST) Sd_User_Malformed_Msg_Fp =
    &[!"string(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackMalformedMsg/Sd_MalformedMsgCallBack)"!];
[!ENDIF!]

[!IF "not(node:empty(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackSubscribeNackReceived/Sd_SubscribeNackReceivedCallBack))"!]
CONST(Sd_User_SubscribeNackReceived_FpType, SD_APPL_CONST) Sd_User_SubscribeNackReceived_Fp =
    &[!"string(as:modconf('Sd')[1]/SdGeneral/VendorSpecific/Sd_SendDataBackSubscribeNackReceived/Sd_SubscribeNackReceivedCallBack)"!];
[!ENDIF!]

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]===========================================*/


