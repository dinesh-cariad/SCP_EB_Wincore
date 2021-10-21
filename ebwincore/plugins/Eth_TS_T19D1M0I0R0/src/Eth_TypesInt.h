
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

#ifndef ETH_TYPESINT_H
#define ETH_TYPESINT_H


/*==================[inclusions]=============================================*/


/*==================[macros]=================================================*/


/*==================[type definitions]=======================================*/
typedef struct /* Eth_CtrlType */ {
    VAR( uint8, TYPEDEF ) CtrlRxBufTotal;
    VAR( uint8, TYPEDEF ) CtrlTxBufTotal;
    VAR( uint8, TYPEDEF ) QueRxBeIdx;
    VAR( uint8, TYPEDEF ) CtrlEnableMii;
    VAR( uint8, TYPEDEF ) CtrlEnableRxInterrupt;
    VAR( uint8, TYPEDEF ) CtrlEnableTxInterrupt;
} Eth_CtrlType;

/** \brief Dummy struct for Eth_Init() */
typedef struct /* Eth_ConfigType */ {
    VAR( uint32, TYPEDEF ) PlatformSignature; /* Used to validate the platform */
    VAR( uint32, TYPEDEF ) LcfgSignature; /* Used to validate the post build configuration against the link time configuration */
    VAR( uint32, TYPEDEF ) CfgSignature; /* Used to validate the post build configuration against the precompile time configuration */
    VAR( uint32, TYPEDEF ) PublicInfoSignature; /* Used to validate Public information */
    P2CONST(Eth_CtrlType, AUTOMATIC, ETH_APPL_CONST)  pCtrls; /* reference to eth controller configuration */
    VAR( uint8, TYPEDEF ) nNumCtrls; /* number of Eth controllers */
} Eth_ConfigType;

/*==================[external function declarations]=========================*/


/*==================[internal function declarations]=========================*/


/*==================[external constants]=====================================*/


/*==================[internal constants]=====================================*/


/*==================[external data]==========================================*/


/*==================[internal data]==========================================*/


/*==================[external function definitions]==========================*/


/*==================[internal function definitions]==========================*/


#endif /* ETH_TYPESINT_H */

/*==================[end of file]============================================*/

