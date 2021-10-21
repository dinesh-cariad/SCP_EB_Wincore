/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRARTP_ENCODE_INT_H
#define FRARTP_ENCODE_INT_H


 /*==================[includes]================================================*/

/*==================[macros]================================================*/

/*==================[type definitions]=========================*/

/*==================[external data declarations]=========================*/

/*==================[external function declarations]=========================*/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>


/**
 * \brief initializes SW-units state data
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_Encode_init
(
  void
);

/**
 * \brief SW-units main function
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_Encode_mainFunction
(
  void
);

/**
 * \brief Cancel the transmission of PDUs scheduled for a specific connection
 *
 * \param[in] actConnIdx        active connection index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_Encode_cancelTx
(
  FrArTp_ActConnIdxType actConnIdx
);

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>


#endif /* ifndef FRARTP_ENCODE_INT_H */
/*==================[end of file]===========================================*/
