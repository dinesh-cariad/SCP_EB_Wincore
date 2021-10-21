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
[!CODE!]
#ifndef ETH_CFG_H
#define ETH_CFG_H

/*==================[includes]==============================================*/

#include <Std_Types.h>              /* Autosar standard data types */
#include <Eth_CfgInt.h>             /* Vendor specific configuration */

/* It is required by AUTOSAR include the headerfile that contains the post-build/link-time 
* symbolic IDs here 
* This mixture of config classes can be omitted for module internal purposes 
*/
#ifndef ETH_NO_CFGCLASSMIX_REQUIRED
#include <Eth_SymbolicNames_PBcfg.h>
#endif /* ETH_NO_CFGCLASSMIX_REQUIRED */

#undef ETH_NO_CFGCLASSMIX_REQUIRED

/*==================[macros]================================================*/

/** \brief Pre-processor switch for enabling development error detection.
 */
#define ETH_DEV_ERROR_DETECT [!//
  [!IF "EthGeneral/EthDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch for enabling version info API support.
 */
#define ETH_GET_VERSION_INFO [!//
  [!IF "EthGeneral/EthVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Pre-processor switch for enabling TSN (Time Sensitive Networking IEEE 1588 time 
** synchroniszation protocol) API
*/
/* !LINKSTO Eth.ASR42.SWS_Eth_00210,1,
            Eth.ASR42.SWS_Eth_00211,1,
            Eth.ASR42.SWS_Eth_00212,1,
            Eth.ASR42.SWS_Eth_00213,1,
            Eth.ASR42.SWS_Eth_00214,1,
            Eth.ASR42.SWS_Eth_00215,1 
*/ 
#define ETH_GLOBAL_TIME_SUPPORT  [!//
[!IF "EthGeneral/EthGlobalTimeSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

 /** \brief Pre-processor switch for enabling Update Physical Address Filter API support.
  */
#define ETH_UPDATE_PHYS_ADDR_FILTER_SUPPORT  [!//
[!IF "EthGeneral/EthUpdatePhysAddrFilter = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

 /** \brief Pre-processor switch for TX interrupt support. This driver does not support interrupts.
  */
#define ETH_ENABLE_TX_INTERRUPT_SUPPORT STD_OFF

/** \brief Pre-processor switch for enabling rx interrupt support for user callout processing
    (not supported). */
#define ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT STD_OFF

/** \brief Pre-processor switch for enabling tx interrupt support for user callout processing 
    (not supported). */
#define ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT STD_OFF

/** \brief Defines the number of eth controllers.
 * !LINKSTO Eth.ASR41.SWS_Eth_00003, 1
 */
#define ETH_CTRL_NO    [!"num:i(count(EthConfigSet/*[1]/EthCtrlConfig/*))"!]U

/** \brief Defines the maximum value of Config Index.
 **        Note: This is defined just to enable the Det checks.
 */
#define ETH_MAX_CFGIDX       0U


/*==================[type definitions]======================================*/


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_CFG_H */
/*==================[end of file]===========================================*/
[!ENDCODE!]

