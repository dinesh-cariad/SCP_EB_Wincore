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

#ifndef SOAD_04_TCPSM_INT_H
#define SOAD_04_TCPSM_INT_H

/* Note: This file was generated from Uml diagram with UmlToSM. Do not change this file! */

/*==================[inclusions]============================================*/
#include <SoAd_04_TcpSM_User.h> /* Unit public header file. */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

/** \brief State machine trigger: Connecting
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 * \return Returns if a transition is executed.
 * \retval TRUE - A transition is executed.
 * \retval FALSE - No transition is executed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Connecting
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief State machine trigger: Closed
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 * \return Returns if a transition is executed.
 * \retval TRUE - A transition is executed.
 * \retval FALSE - No transition is executed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Closed
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief State machine trigger: LoClosing
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 * \return Returns if a transition is executed.
 * \retval TRUE - A transition is executed.
 * \retval FALSE - No transition is executed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_LoClosing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief State machine trigger: UpClosing
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 * \return Returns if a transition is executed.
 * \retval TRUE - A transition is executed.
 * \retval FALSE - No transition is executed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_UpClosing
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

/** \brief State machine trigger: Connected
 *
 * \param[in] SoConPtr - Pointer to socket connection configuration.
 * \param[in/out] SoConStatePtr - Pointer to socket state.
 *
 * \return Returns if a transition is executed.
 * \retval TRUE - A transition is executed.
 * \retval FALSE - No transition is executed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, SOAD_CODE) SoAd_Int_04_TcpSM_Connected
(
  CONSTP2CONST(SoAd_SoConType, AUTOMATIC, SOAD_APPL_DATA) SoConPtr,
  CONSTP2VAR(SoAd_SoConStateType, AUTOMATIC, SOAD_APPL_DATA) SoConStatePtr
);

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>

#endif /* ifndef(SOAD_04_TCPSM_INT_H) */
/*==================[end of file]===========================================*/
