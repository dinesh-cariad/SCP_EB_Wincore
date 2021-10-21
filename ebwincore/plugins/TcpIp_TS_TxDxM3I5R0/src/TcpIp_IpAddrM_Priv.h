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

#ifndef TCPIP_IPADDRM_PRIV_H
#define TCPIP_IPADDRM_PRIV_H

/*==================[inclusions]============================================*/
#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_IpAddrM_Int.h>  /* module internal header file */
#include <TcpIp_Trace.h>        /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/** \brief Number of configured Socket owners. equals \a TcpIp_NumSockOwner */
#define TCPIP_IPADDRM_NUMCONFSOCKOWNER TcpIp_NumSockOwner

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/*-------------------[SW-unit interface function declarations]---------------*/

/*-------------------[SW-unit private function declarations]---------------*/

/** \brief IpAddrM main function. Process event flags.
*/
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_mainFunction_localAddr(TcpIp_LocalAddrIdType localAddrId);

/** \brief Get local addr Id state
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_getIntIfState(TcpIp_LocalAddrIdType localAddrId);

/** \brief Get Ip configuration
 *
 * Output Ip configuration (SW-unit internal API)
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIntIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);

/** \brief Get Ip address
 *
 * Output Ip address (SW-unit internal API)
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_getIntIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);


/** \brief returns whether there is auto Assignment enabled this local address identifier or not
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] type                assignment type
 *
 * \return
 * \retval TRUE   auto assignment is enabled for this local address assignment
 * \retval FALSE  auto assignment is disabled for this local address assignment
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_autoAssignment(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType type);


/** \brief Initializes IpAddrM CtrlSM general data structures
 *
 * This function initializes the general data structures of SW-unit IpaddrM controller state machine (CtrlSM).
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_init(void);



/** \brief Initializes IpAddrM CtrlSM controller specific data structures
 *
 * This function initializes the controller specific data structures of SW-unit IpaddrM controller
 * state machine (CtrlSM).
 *
 * \param[in] ctrlIdx     controller index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_initCtrl(uint8 ctrlIdx);



/** \brief CtrlSM trigger function: request new controller state
 *
 * \param[in] ctrlIdx         controller index
 * \param[in] requestedState  new state requested
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_request(uint8 ctrlIdx, TcpIp_StateType requestedState);



/** \brief CtrlSM trigger function: decoupled update of state machine
 *
 * \param[in] ctrlIdx         controller index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_update(uint8 ctrlIdx);



/** \brief CtrlSM trigger function: IP address has been assigned to this controller
 *
 * \param[in] ctrlIdx         controller index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_assigned(uint8 ctrlIdx);



/** \brief CtrlSM trigger function: IP address has been unassigned from this controller
 *
 * \param[in] ctrlIdx         controller index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_unassigned(uint8 ctrlIdx);



/** \brief returns current state machine state
 *
 * \param[in] ctrlIdx         controller index
 *
 * \return current state machine state of the controller
 */
TS_MOD_PRIV_DECL FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_IpAddrM_CtrlSM_getCurrentState(uint8 ctrlIdx);





/** \brief Initializes IpAddrM LocalAddrSM local address Id specific data structures
 *
 * This function initializes the local address Id specific data structures of SW-unit
 * IpaddrM local address state machine (LocalAddrSM)
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_init(TcpIp_LocalAddrIdType localAddrId);



/** \brief LocalAddrSM trigger function: start address assignment
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_startAssign(TcpIp_LocalAddrIdType localAddrId);



/** \brief LocalAddrSM trigger function: stop address assignment
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_stopAssign(TcpIp_LocalAddrIdType localAddrId);



/** \brief LocalAddrSM trigger function: hold address assignment (ethernet link is lost)
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_holdAssign(TcpIp_LocalAddrIdType localAddrId);



/** \brief LocalAddrSM trigger function: continue address assignment (opposite of hold)
 *
 * \param[in] localAddrId         local address identifier
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_continueAssign(TcpIp_LocalAddrIdType localAddrId);



/** \brief LocalAddrSM trigger function: an address has been assigned successfully
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_assigned(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);



/** \brief LocalAddrSM trigger function: an address has been unassigned
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_LocalAddrSM_unassigned(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);

/** \brief Initializes IpAddrM AssignSM specific data structures
 *
 * This function initializes the data structures of SW-unit IpaddrM
 * address assignment state machine (AssignSM)
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_init(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);



/** \brief AssignSM trigger function: start address assignment
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_startAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);



/** \brief AssignSM trigger function: stop address assignment
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_stopAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);




/** \brief AssignSM trigger function: request a specific address for assignment
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_requestAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);



/** \brief AssignSM trigger function: release a address assignment
 *
 * \param[in] localAddrId         local address identifier
 * \param[in] method              assignment method
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_releaseAssign(TcpIp_LocalAddrIdType localAddrId, TcpIp_IpAddrAssignmentType method);



/** \brief get Ip configuration of local address and assignment method
 *
 * This API function outputs the IP configuration of a local address/assignment method
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 * \param[out] gatewayPtrPtr  Pointer which is set to the default gatewway IP address
 * \param[out] netmaskIpPtrPtr  Pointer which is set to the netmask
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getIpConfig
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) gatewayIpPtrPtr,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) netmaskIpPtrPtr
);



/** \brief get Ip address of local address and assignment method
 *
 * This API function outputs the IP address of a local address/assignment method
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 * \param[out] addressIpPtrPtr  Pointer which is set to the assigned IP address
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2VAR(TcpIp_IpAddrConstPtr,AUTOMATIC,TCPIP_APPL_DATA) addressIpPtrPtr
);



/** \brief get status whether assignment is achieved
 *
 * This API function outputs the netmask of a local address/assignment method
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 *
 * \return assignment state
 */
TS_MOD_PRIV_DECL FUNC(boolean, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_isAssigned
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method
);



/** \brief indicate that this is the active assignment method
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 * \param[in] state           TRUE: assignment is active, FALSE: assignment is inactive
 *
 */
TS_MOD_PRIV_DECL FUNC(void, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_assignmentActive
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    boolean state
);

/** \brief request an IP address which shall be assigned
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 * \param[in] LocalIpAddrPtr  Pointer to requested IP address
 *
 * \return Result of operation
 * \retval E_OK      The request has been accepted
 * \retval E_NOT_OK  The request has not been accepted
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_requestIpAddress
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) LocalIpAddrPtr
);

#if (TCPIP_IPV6_ENABLED == STD_ON)

/** \brief Returns the current state of the ip address (preferred, deprecated)
 *
 * \param[in] localAddrId     Id of local IP address
 * \param[in] method          Assignment method
 *
 * \return Result of operation
 * \retval TCPIP_IPADDRM_ADDRSTATE_PREFERRED  ip address is preferred
 * \retval TCPIP_IPADDRM_ADDRSTATE_DEPRECATED ip address is deprecated
 */
TS_MOD_PRIV_DECL FUNC(uint8, TCPIP_CODE) TcpIp_IpAddrM_AssignSM_getAddrState
(
    TcpIp_LocalAddrIdType localAddrId,
    TcpIp_IpAddrAssignmentType method
);

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* ifndef TCPIP_IPADDRM_PRIV_H */
/*==================[end of file]===========================================*/
