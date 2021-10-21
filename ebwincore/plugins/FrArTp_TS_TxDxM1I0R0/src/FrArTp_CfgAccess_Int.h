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
#ifndef FRARTP_CFGACCESS_INT_H
#define FRARTP_CFGACCESS_INT_H


 /*==================[includes]================================================*/

#include <FrArTp_Int.h>            /* FrArTp pre-compile-time configuration */
#if (!defined FRARTP_NO_PBCFG_REQUIRED) && (!defined FRARTP_NO_CFG_REQUIRED)
#include <FrArTp_PBcfg.h>            /* FrArTp pre-compile-time configuration */
#endif

/*==================[macros]================================================*/

#ifdef FRARTP_PRECOMPILE_TIME_PBCFG_ACCESS

#define FRARTP_GET_CONFIG_ADDR(type, element)\
(FrArTp_ConfigLayout.a ## element)

#else

/** \brief Define FRARTP_GET_CONFIG_ADDR macro for getting a reference to a config element, */
/* Deviation MISRAC2012-1 */
#define FRARTP_GET_CONFIG_ADDR(type, element)\
(TS_UNCHECKEDGETCFG(FrArTp_PBcfgPtr, type, FRARTP, (FrArTp_PBcfgPtr->p ## element)))

#endif /* FRARTP_PRECOMPILE_TIME_PBCFG_ACCESS */

/*==================[type definitions]=========================*/

/*==================[external data declarations]=========================*/

/*==================[external function declarations]=========================*/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/**
 * \brief Initialize the configuration access unit
 *
 * \param[in] cfgPtr    Pointer to the postbuild configuration
 *
 * \return E_OK         Unit has been initialized successfully
 * \return E_NOT_OK     Unit initialization failed
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_CfgAccess_init
(
    P2CONST(FrArTp_ConfigType, FRARTP_VAR_CLEARED, FRARTP_APPL_CONST) cfgPtr
);

#if (FRARTP_DEV_ERROR_DETECT == STD_ON)
/**
 * \brief Check Rx PduId
 *
 * Checks if the Rx PduId is valid for the current FrArTp configuration.
 *
 * \param[in] rxPduIdx        This parameter contains the unique identifier of the received Pdu
 *
 * \return TRUE         The value of rxPduIdx is valid for the current configuration
 * \return FALSE        The value of rxPduIdx is invalid for the current configuration
 */
TS_MOD_PRIV_DECL FUNC(boolean, FRARTP_CODE) FrArTp_CfgAccess_isValid_RxPduIdx
(
  PduIdType rxPduIdx
);



/**
 * \brief Check Tx PduId
 *
 * Checks if the Tx PduId is valid for the current FrArTp configuration.
 *
 * \param[in] txPduIdx        This parameter contains the unique identifier of the received Pdu
 *
 * \return TRUE         The value of txPduIdx is valid for the current configuration
 * \return FALSE        The value of txPduIdx is invalid for the current configuration
 */
TS_MOD_PRIV_DECL FUNC(boolean, FRARTP_CODE) FrArTp_CfgAccess_isValid_TxPduIdx
(
    PduIdType txPduIdx
);
#endif /* (FRARTP_DEV_ERROR_DETECT == STD_ON) */



/**
 * \brief get channel config data structure
 *
 * Returns a pointer to the config data structure of type FrArTp_ChannelCfgType for the channel the
 * passed Rx PduId is part of.
 *
 * \param[in] rxPduIdx        This parameter contains the unique identifier of the received Pdu
 *
 * \return                    Pointer to channel config data structure
 */
TS_MOD_PRIV_DECL FUNC_P2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getChannelCfgPtr_byRxPduIdx
(
  FrArTp_RxPduIdxType rxPduIdx
);



/**
 * \brief get channel config data structure
 *
 * Returns a pointer to the config data structure of type FrArTp_ChannelCfgType for the channel the
 * connection index connIdx is part of.
 *
 * \param[in] connIdx         Connection index
 *
 * \return                    Pointer to channel config data structure
 */
TS_MOD_PRIV_DECL FUNC_P2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx
(
  FrArTp_ConnIdxType connIdx
);



/**
 * \brief Maps a received message to a connection index contained in configuration
 *
 * Returns The connection index of a rceeived message, based on rxPduIdx, localAddress and remoteAddress.
 *
 * \param[in] rxPduIdx        This parameter contains the unique identifier of the received Pdu
 * \param[in] localAddress    address of the local ECU within the connection
 * \param[in] remoteAddress   address of the remote ECU within the connection
 *
 * \return                    Connection Index. FRARTP_CONNECTIONIDX_INVALID means no connection has been found.
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byAddress
(
  FrArTp_RxPduIdxType rxPduIdx,
  uint16 localAddress,
  uint16 remoteAddress
);



/**
 * \brief Map tx handle ID to connection ID
 *
 * Maps the txSduId to the connection index and returns it.
 *
 * \param[in] txSduId         transmit handle ID
 *
 * \return                    connection index
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byTxSduId
(
  PduIdType txSduId
);



/**
 * \brief Map rx handle ID to connection ID
 *
 * Maps the rxSduId to the connection index and returns it.
 *
 * \param[in] rxSduId         receive handle ID
 *
 * \return                    connection index
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getConnectionIdx_byRxSduId
(
  PduIdType rxSduId
);



/**
 * \brief get tx pool config data structure
 *
 * Returns a pointer to the config data structure of type FrArTp_TxPoolCfgType for the tx pool txPoolIdx.
 *
 * \param[in] txPoolIdx       Transmit pool index
 *
 * \return                    Pointer to tx pool config data structure
 */
TS_MOD_PRIV_DECL FUNC_P2CONST(FrArTp_TxPoolCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getTxPoolCfgPtr
(
  FrArTp_TxPoolIdxType txPoolIdx
);



/**
 * \brief get tx pdu config data structure
 *
 * Returns a pointer to the config data structure of type FrArTp_TxPduCfgType for the tx pdu txPduIdx.
 *
 * \param[in] txPduIdx        Transmit pdu index
 *
 * \return                    Pointer to tx pdu config data structure
 */
TS_MOD_PRIV_DECL FUNC_P2CONST(FrArTp_TxPduCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getTxPduCfgPtr
(
  FrArTp_TxPduIdxType txPduIdx
);



/**
 * \brief get connection config data structure
 *
 * Returns a pointer to the config data structure of type FrArTp_ConnectionCfgType for the connection connIdx.
 *
 * \param[in] connIdx         connection index
 *
 * \return                    Pointer to connection config data structure
 */
TS_MOD_PRIV_DECL FUNC_P2CONST(FrArTp_ConnectionCfgType, AUTOMATIC, FRARTP_CODE) FrArTp_CfgAccess_getConnectionCfgPtr
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief Get the total number of configured transmit PDU pools
 *
 * \return Number of transmit PDU pools
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_TxPoolIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumTxPools
(
  void
);

/**
 * \brief Get the total number of configured connections
 *
 * \return Number of connections
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumConn
(
  void
);

/**
 * \brief Get the configured main function period in milliseconds
 *
 * \return Main function period
 */
TS_MOD_PRIV_DECL FUNC(uint16, FRARTP_CODE) FrArTp_CfgAccess_getMainFunctionPeriodMS
(
  void
);

#if (FRARTP_DEV_ERROR_DETECT == STD_ON)

/**
 * \brief Get the total number of configured transmit SDUs
 *
 * \return Number of transmit SDUs
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumTxSdus
(
  void
);

/**
 * \brief Get the total number of configured receive SDUs
 *
 * \return Number of receive SDUs
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_CfgAccess_getNumRxSdus
(
  void
);

#endif /* (FRARTP_DEV_ERROR_DETECT == STD_ON) */

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>


#endif /* ifndef FRARTP_CFGACCESS_INT_H */
/*==================[end of file]===========================================*/
