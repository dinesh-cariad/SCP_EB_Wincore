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
[!VAR "UseOs"="node:exists(as:modconf('Os')[1]) and as:modconf('Os')[1]/CommonPublishedInformation/SwMajorVersion >= 5"!][!//

#ifndef ETH_CFGINT_H
#define ETH_CFGINT_H

/*==================[includes]==============================================*/

#include <Std_Types.h>              /* Autosar standard data types */
[!IF "$UseOs"!][!//
#include <Os.h>                     /* OS_WINDOWSGoingToUseWindowsService, ... */
[!ENDIF!]
/*==================[macros]================================================*/


/** \brief Number of ethernet device to listen on
 *
 * Depends on local installation and configuration of host PC. Select the
 * Windows loopback device (after you have installed it,
 * http://support.microsoft.com/kb/839013) for local testing */
#define ETH_WINPCAP_DEV_NUM [!"EthGeneral/VendorSpecific/EthEthernetAdapter"!]U

/** Ethernet address be used on the simulated device */
#define ETH_WINPCAP_ETH_ADDR {[!LOOP "text:split(EthConfigSet/*[1]/EthCtrlConfig/*[1]/EthCtrlPhyAddress,':')"!][!LOOP "text:split(.,'-')"!]0x[!"."!]U,[!ENDLOOP!][!ENDLOOP!]}

/** Length of the transmit buffers (for payload and ethernet protocol header) */
#define ETH_WINPCAP_TXBUFLEN [!"EthConfigSet/*[1]/EthCtrlConfig/*[1]/EthCtrlTxBufLenByte"!]U

/** Number of transmit buffers */
#define ETH_WINPCAP_TXBUFTOTAL [!"EthConfigSet/*[1]/EthCtrlConfig/*[1]/EthTxBufTotal"!]U

[!IF "$UseOs"!][!//
/** \note Since Version 5 the Os must be informed about access to windows services.
 *  \see OS_WINDOWSGoingToUseWindowsService() and OS_WINDOWSFinishedUsingWindowsService() */

[!ENDIF!][!//
/** \brief Enter a section of code in which a Windows service is used */
#define ETH_ENTER_OS_WINDOWS_SERVICE_AREA() [!IF "$UseOs"!]os_intstatus_t is = OS_WINDOWSGoingToUseWindowsService()[!ENDIF!]

/** \brief Exit the section of code in which a Windows service is used */
#define ETH_EXIT_OS_WINDOWS_SERVICE_AREA() [!IF "$UseOs"!]OS_WINDOWSFinishedUsingWindowsService(is)[!ENDIF!]

/** \brief No PBCfg is supported */
#define ETH_PBCFGM_SUPPORT_ENABLED STD_OFF

/** \brief No configuration relocatable is supported */
#define ETH_RELOCATABLE_CFG_ENABLE STD_OFF

/** \brief No defensive programming is supported */
#define ETH_PRECONDITION_ASSERT_ENABLED STD_OFF

/** \brief No defensive programming is supported */
#define ETH_POSTCONDITION_ASSERT_ENABLED STD_OFF

/** \brief No defensive programming is supported */
#define ETH_INVARIANT_ASSERT_ENABLE STD_OFF

/** \brief No defensive programming is supported */
#define ETH_STATIC_ASSERT_ENABLED STD_OFF

/** \brief No defensive programming is supported */
#define ETH_UNREACHABLE_CODE_ASSERT_ENABLED STD_OFF

/** \brief No defensive programming is supported */
#define ETH_INVARIANT_ASSERT_ENABLED STD_OFF

/** \brief Enable MII support (backward compatiblity) */
#define ETH_ENABLE_MII_SUPPORT STD_ON

/** \brief Pre-processor switch that mappes to Eth MII API enable switch. */
#define ETH_ANY_MII_SUPPORT ETH_ENABLE_MII_SUPPORT

/** \brief Pre-processor switch for selecting the role of this Eth driver. Hard coded to Eth driver.
*/
#define ETH_VIRT_ROLE_ETH        0U
#define ETH_VIRT_ROLE_ETH_VSWT   1U
#define ETH_VIRT_ROLE_ETH_VIRT   2U
#define ETH_VIRT_ROLE           ETH_VIRT_ROLE_ETH

/** \brief Pre-processor switch for selecting the supported API of this Eth driver.
 * The WinCore Eth driver supports only AUTOSAR SWS 422 API.
 */
#define ETH_SUPPORT_API_ASR422    0U
#define ETH_SUPPORT_API_ASR430    1U
#define ETH_SUPPORT_API           ETH_SUPPORT_API_ASR422

/** \brief Define the number of priority per eth controller
 *  The WinCore Eth driver do not support any priority, thus hard coded to 1 (for best effort).
 */
#define ETH_QOS_PRIORITY_NO 1U

/** \brief Disable of feature re-transmit (not supported by WinCore Eth driver) */
#define ETH_SUPPORT_RETRANSMIT STD_OFF

/** \brief Disable of feature buffer list support (not supported by WinCore Eth driver) */
#define ETH_BUFFER_LIST_SUPPORT STD_OFF

#define ETH_CFG_SIGNATURE [!"asc:getConfigSignature(as:modconf('Eth')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') ])"!]U /* Compile time verification value */

#define ETH_PUBLIC_INFO_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('Eth')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('Eth')[1]/CommonPublishedInformation/Release))"!]U /* PublicInfoSignature */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[includes]==============================================*/

#endif /* ifndef ETH_CFGINT_H */

/*==================[end of file]===========================================*/
[!ENDCODE!]
