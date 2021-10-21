/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <Eth_PBcfg.h>
#define TS_RELOCATABLE_CFG_ENABLE STD_OFF
#define TS_PB_CFG_NAME Eth_ConfigLayout
#define TS_PB_CFG_LAYOUT_TYPE Eth_ConstConfigLayoutType
#include <TSPBConfig_Init.h>

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eth_MemMap.h>

Eth_ConstConfigLayoutType Eth_ConfigLayout = {
    { /* RootCfg */
        UINT32_C( [!"asc:getPlatformSignature()"!] ), /* PlatformSignature */
        UINT32_C( [!"asc:getConfigSignature(as:modconf('Eth')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!] ), /* LcfgSignature */
        UINT32_C( [!"asc:getConfigSignature(as:modconf('Eth')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!] ), /* CfgSignature */
        UINT32_C( [!"asc:getConfigSignature(node:difference(as:modconf('Eth')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('Eth')[1]/CommonPublishedInformation/Release))"!] ), /* PublicInfoSignature */
        TS_MAKEREF2CFG( aCtrls[0] ), /* pCtrls */
        UINT8_C( 1 ) /* nNumCtrls */
    },
    { /* aCtrls */
        { /* aCtrls[0] */
            UINT8_C( [!"EthConfigSet/*[1]/EthCtrlConfig/*[1]/EthRxBufTotal"!] ), /* RxBufTotal */
            UINT8_C( [!"EthConfigSet/*[1]/EthCtrlConfig/*[1]/EthTxBufTotal"!] ), /* TxBufTotal */
            UINT8_C( 0 ), /* QueRxBeIdx */
            UINT8_C( 1 ), /* CtrlEnableMii */
            UINT8_C( 0 ), /* CtrlEnableRxInterrupt */
            UINT8_C( 0 ), /* CtrlEnableTxInterrupt */
        }
    }
};

#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Eth_MemMap.h>

/*==================[internal constants]=====================================*/


/*==================[external data]==========================================*/


/*==================[internal data]==========================================*/


/*==================[external function definitions]==========================*/


/*==================[internal function definitions]==========================*/


/*==================[end of file]============================================*/

