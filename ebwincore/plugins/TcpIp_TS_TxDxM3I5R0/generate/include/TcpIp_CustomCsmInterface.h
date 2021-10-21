/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TCPIP_CUSTOMCSMINTERFACE_H
#define TCPIP_CUSTOMCSMINTERFACE_H


/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpCustomCsmInterfaceHeaderFile)"!]
#include <[!"as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpCustomCsmInterfaceHeaderFile"!]>
[!ENDIF!]

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* TCPIP_CUSTOMCSMINTERFACE_H */
