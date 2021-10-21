/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOAD_02_SOCKETCON_INT_H
#define SOAD_02_SOCKETCON_INT_H

/*==================[includes]================================================*/
#include <TSAutosar.h> /* EB specific standard types. */
#include <ComStack_Types.h>
#include <TcpIp.h>
/* ======= ======= */
#include <SoAd_Types.h> /* Module public types. */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]=========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_02_ULSoConModeChg
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  SoAd_SoConModeType Mode
);

TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_02_CleanUpSoCon
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief Service to close a socket
 **
 ** \param[in]    SoConPtr - Pointer to the configuration of the socket connection
 ** \param[inout] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[in] Abort - Indicates if the close is caused by an abort from upper layer.
 **
 **/
TS_MOD_PRIV_DECL FUNC(void, SOAD_CODE) SoAd_Int_02_CloseSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  boolean Abort
);

/** \brief Requests a new socket from the TcpIp module
 **
 ** \param[in]    SoConPtr - Pointer to the configuration of the socket connection
 ** \param[inout] SoConStatePtr - Pointer to the state information of the socket connection
 ** \param[out]   SocketIdPtr - Pointer to the memory where the Id of the new socket shall be
 **                             stored.
 ** \returns      Std_ReturnType
 ** \retval       E_OK     The socket request was successful
 ** \retval       E_NOT_OK The socket request was not successful, e.g. no free socket available
 **/
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestTcpIpSocket
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr,
  CONSTP2VAR(TcpIp_SocketIdType, AUTOMATIC, SOAD_APPL_DATA) SocketIdPtr
);


#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

#endif /* ifndef(SOAD_02_SOCKETCON_INT_H) */
/*==================[end of file]===========================================*/
