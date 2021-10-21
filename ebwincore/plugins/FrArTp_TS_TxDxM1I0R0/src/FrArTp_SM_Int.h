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
#ifndef FRARTP_SM_INT_H
#define FRARTP_SM_INT_H


 /*==================[includes]================================================*/

#include <FrArTp_Int.h>            /* FrArTp generic interfaces */
#include <FrArTp_Encode_Int.h>     /* Module internal encode unit interfaces */

/*==================[macros]================================================*/

/** \brief state IDLE */
#define FRARTP_SM_RXSTATE_IDLE                                              0U

/** \brief state CHECKBUFFER */
#define FRARTP_SM_RXSTATE_CHECKBUFFER                                       1U

/** \brief state CTS substate WAITPDU */
#define FRARTP_SM_RXSTATE_CTS_WAITPDU                                       2U

/** \brief state CTS substate TXPENDING */
#define FRARTP_SM_RXSTATE_CTS_TXPENDING                                     3U

/** \brief state OVFLW substate WAITPDU */
#define FRARTP_SM_RXSTATE_OVFLW_WAITPDU                                     4U

/** \brief state OVFLW substate TXPENDING */
#define FRARTP_SM_RXSTATE_OVFLW_TXPENDING                                   5U

/** \brief state WAIT substate WAITPDU */
#define FRARTP_SM_RXSTATE_WAIT_WAITPDU                                      6U

/** \brief state WAIT substate TXPENDING */
#define FRARTP_SM_RXSTATE_WAIT_TXPENDING                                    7U

/** \brief state WAITDATA */
#define FRARTP_SM_RXSTATE_WAITDATA                                          8U

/**\brief SN max value */
#define FRARTP_MAX_SEQUENCE_VALUE                                          15U

/** \brief Flow control frame flow status values */
#define FRARTP_SM_FC_FS_CTS                                                0x0U
#define FRARTP_SM_FC_FS_WT                                                 0x1U
#define FRARTP_SM_FC_FS_OVFLW                                              0x2U

/** \brief Frame types (independent of ISO/L4G) */
#define FRARTP_SM_FT_SINGLE_FRAME                                             0x0U
#define FRARTP_SM_FT_FIRST_FRAME                                              0x1U
#define FRARTP_SM_FT_CONSECUTIVE_FRAME                                        0x2U

/** \brief start timer */
#define FrArTp_SM_Rx_StartTimer(actConnIdx, time) \
        FrArTp_SM_actConn[(actConnIdx)].rx.timer = (time)

/** \brief stop timer */
#define FrArTp_SM_Rx_StopTimer(actConnIdx) \
        FrArTp_SM_actConn[(actConnIdx)].rx.timer = (0U)

/** \brief state IDLE */
#define FRARTP_SM_TXSTATE_IDLE                                              0U

/**\brief state WAITPDU */
#define FRARTP_SM_TXSTATE_WAITPDU                                           1U

/**\brief state TXPENDING */
#define FRARTP_SM_TXSTATE_TXPENDING                                         2U

/** \brief state WAITTX */
#define FRARTP_SM_TXSTATE_WAITTX                                            3U

/** \brief state WAITFC */
#define FRARTP_SM_TXSTATE_WAITFC                                            4U

/** \brief start timer */
#define FrArTp_SM_Tx_StartTimer(actConnIdx, time) \
        FrArTp_SM_actConn[(actConnIdx)].tx.timer = (time)

/** \brief stop timer */
#define FrArTp_SM_Tx_StopTimer(actConnIdx) \
        FrArTp_SM_actConn[(actConnIdx)].tx.timer = (0U)

/*==================[type definitions]======================================*/

/** \brief Describes an active receiving connection */
typedef struct
{
  uint32 available; /**< size of buffer currently available */
  uint32 remaining; /**< number of bytes still to process */
  uint16 timer;     /**< timer variable for this active connections */
  uint8 state;      /**< state variable */
  uint8 frameCount; /**< Number of frames already received within the current block */
  uint8 seq;        /**< sequence counter value */
  uint8 count;      /**< counter for repetitive actions (e.g. wait transmissions) */
}FrArTp_ActRxConnStateType;

/** \brief Describes an active transmitting connection */
typedef struct
{
  uint32 available; /**< size of buffer currently available */
  uint32 remaining; /**< number of bytes still to process */
  uint16 timer;     /**< timer variable for this active connections */
  uint8 stMinTicks; /**< value of stMin in mainfunction ticks */
  uint8 state;      /**< state variable */
  uint8 frameCount; /**< Number of frames still to be transmitted within the current block */
  uint8 seq;        /**< sequence counter value */
  uint8 frameType;  /**< single/first/consecutive frame */
  boolean ctsExpected; /**< states that wether a FC CTS frame is expected */
}FrArTp_ActTxConnStateType;

/** \brief Describes an active connection that can be full duplex (transmitting and receiving) */
typedef struct
{
  FrArTp_ActRxConnStateType rx; /**< description of the active receiving connection */
  FrArTp_ActTxConnStateType tx; /**< description of the active transmitting connection */
  FrArTp_ConnIdxType connIdx; /**< connection index of the active connection */
}FrArTp_ActConnType;

/** \brief Describes properties of a connection for segmented reception  */
typedef struct
{
  uint8 BS; /**< the blocksize for reception */
  uint8 stMin; /**< the minimum seperation time for reception */
}FrArTp_ConnType;

/*==================[external data declarations]=========================================*/

#define FRARTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

/** \brief Properties of active connections */
extern VAR(FrArTp_ActConnType,FRARTP_VAR) FrArTp_SM_actConn[FRARTP_ACTCONN_MAX];

/** \brief Properties of connections */
extern VAR(FrArTp_ConnType,FRARTP_VAR) FrArTp_SM_Conn[FRARTP_CONN_MAX];

#define FRARTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrArTp_MemMap.h>

/*==================[external function declarations]=====================================*/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/** \brief initializes SW-units state data */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_init
(
  void
);

/** \brief state machine main function interface */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_mainFunction
(
  void
);

/**
 * \brief rx state machine main function
 *
 * \param[in] actConnIdx active connection index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_Rx_mainFunction
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief tx state machine main function
 *
 * \param[in] actConnIdx active connection index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_Tx_mainFunction
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief get connection index of active connection
 *
 * \param[in] actConnIdx        active connection index
 *
 * \return    connection index
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ConnIdxType, FRARTP_CODE) FrArTp_SM_getConnIdx
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief is called by Encode SW-Units in case data has been received
 *
 * If Encode SW-units receives any data (FF, SF, CF) then it calls this event function of the
 * SM SW-unit and passes the received data.
 *
 * \param[in] connIdx        Connection index of the received message
 * \param[in] dataLength     content of the data length control field (SF, FF) or 0 for CF
 * \param[in] payloadLength  length of the data payload in units of bytes (as pointed to by pPayload)
 * \param[in] sequenceNumber sequence number contained in the received message (0 for FF, SF)
 * \param[in] pPayload       pointer to the data payload
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_receivedData
(
  FrArTp_ConnIdxType connIdx,
  uint32 dataLength,
  uint8 payloadLength,
  uint8 sequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload
);

/**
 * \brief is called by Encode SW-Units in case a FC-CTS has been received
 *
 * If Encode SW-units receives FC-CTS then it calls this event function of the
 * SM SW-unit and passes the received flow control parameters.
 *
 * \param[in] connIdx Connection index of the received message
 * \param[in] BS      BlockSize value received
 * \param[in] STMin   STMin value received
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_receivedCTS
(
  FrArTp_ConnIdxType connIdx,
  uint8 BS,
  uint8 STMin
);

/**
 * \brief is called by Encode SW-Units in case a FC-WT has been received
 *
 * If Encode SW-units receives FC-WT then it calls this event function of the
 * SM SW-unit.
 *
 * \param[in] connIdx Connection index of the received message
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_receivedWT
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief is called by Encode SW-Units in case a FC-OVFLW has been received
 *
 * If Encode SW-units receives FC-OVFLW then it calls this event function of the
 * SM SW-unit.
 *
 * \param[in] connIdx Connection index of the received message
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_receivedOVFLW
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief is called by Decode SW-Units in case a FC-InvalidFS has been received
 *
 * If Decode SW-units receives FC-InvalidFS then it calls this event function of the
 * SM SW-unit.
 *
 * \param[in] connIdx Connection index of the received message
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_receivedInvalidFS
(
  FrArTp_ConnIdxType connIdx
);


/**
 * \brief Returns the number of data frames a tx SM wants to transmit
 *
 * returns information for a particular connection, how many data frame are still to be transmitted
 *
 * \param[in] actConnIdx Index of the active Connection
 *
 * \return Number of frames still to be transmitted
 */
TS_MOD_PRIV_DECL FUNC(uint8, FRARTP_CODE) FrArTp_SM_isDataPending
(
  FrArTp_ActConnIdxType actConnIdx
);


/**
 * \brief Return whether Rx-SM wants transmit a FC frame
 *
 * returns information for a particular connection, whether the Receive-State machine needs to
 *  transmit a flow control.
 *
 * \param[in] actConnIdx Index of the active Connection
 *
 * \return 0  No FC frame to transmit
 * \return 1  A FC frame to transmit
 */
TS_MOD_PRIV_DECL FUNC(uint8, FRARTP_CODE) FrArTp_SM_isFCPending
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief Copy data for transmission
 *
 * Copies the data for the transmission of a Tx N-PDU and updates the corresponding state machine.
 *
 * \param[in] actConnIdx                Index of the active Connection
 * \param[in] connIdx                   Index of the connection
 * \param[out] pDataLength              Address to write the data length value to (0 for CF)
 * \param[in/out] pPayloadLength        Payload length derived from channel. Address to write the payload length update of the last frame to
 * \param[out] pSequenceNumber          Address to write the sequence number to
 * \param[out] pPayload                 Address to write the payload to
 * \param[in] remainingSduLength        Remaining length of an pdu, reserved for a payload
 *
 * \return TRUE          data copied successfully, transmission shall be performed
 * \return FALSE         data copy failed, no transmission shall be performed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, FRARTP_CODE) FrArTp_SM_copyData
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx,
  P2VAR(uint32, AUTOMATIC, FRARTP_APPL_DATA) pDataLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayloadLength,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pSequenceNumber,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pPayload,
  PduLengthType remainingSduLength
);

/**
 * \brief Copy the FC parameters for transmission
 *
 * Copies the parameters of a Flow Control (FC) frame for transmission using a Tx N-PDU.
 *
 * \param[in] actConnIdx  Index of the active Connection
 * \param[out] pFrameStatus        Address to write the flow status (FS) to
 * \param[out] pBlockSize        Address to write the block size (BS) to
 * \param[out] pStMin     Address to write the STMin to
 *
 * \return TRUE          parameters copied successfully, transmission shall be performed
 * \return FALSE         parameters copy failed, no transmission shall be performed.
 */
TS_MOD_PRIV_DECL FUNC(boolean, FRARTP_CODE) FrArTp_SM_copyFCParameters
(
  FrArTp_ActConnIdxType actConnIdx,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pFrameStatus,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pBlockSize,
  P2VAR(uint8, AUTOMATIC, FRARTP_APPL_DATA) pStMin
);

/**
 * \brief Cancel a connection reception
 *
 * \param[in] connIdx  connection index
 *
 * \return E_OK       cancellation was successful
 * \return E_NOT_OK   cancellation was not successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_cancelRx
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief Cancel a connection transmission
 *
 * \param[in] connIdx  connection index
 *
 * \return E_OK       cancellation was successful
 * \return E_NOT_OK   cancellation was not successful
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_cancelTx
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief transmission confirmation for flow control transmission
 *
 * \param[in] connIdx  connection index
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_txConfirmationFC
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief transmission confirmation for data transmission
 *
 * \param[in] actConnIdx  active connection index
 * \param[in] connIdx     connection index
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_txConfirmationData
(
  FrArTp_ActConnIdxType actConnIdx,
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief get type of data frame to be transmitted (FF/SF/CF)
 *
 * \param[in] connIdx  connection index
 *
 * \return FRARTP_SM_FT_SINGLE_FRAME        single frame
 * \return FRARTP_SM_FT_FIRST_FRAME         first frame
 * \return FRARTP_SM_FT_CONSECUTIVE_FRAME   consecutive frame
 */
TS_MOD_PRIV_DECL FUNC(uint8, FRARTP_CODE) FrArTp_SM_getTxFrameType
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief request transmission of a Sdu
 *
 * \param[in] connIdx     connection index
 * \param[in] sduLength   total length of Sdu to be transmitted
 * \param[in] frameType   type of the frame to be transmitted
 *
 * \return E_OK       transmission request accepted
 * \return E_NOT_OK   transmission request not accepted
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, FRARTP_CODE) FrArTp_SM_transmit
(
  FrArTp_ConnIdxType connIdx,
  PduLengthType sduLength,
  uint8 frameType
);

/**
 * \brief Find and returns an allocated active connection
 *
 * \param[in] connIdx     connection index
 *
 * \return FRARTP_ACTCONN_INVALID       connection not active
 * \return *                            active connection index for the connection
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_FindActConnIdx
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief Deallocate an active connection
 *
 * \param[in] actConnIdx     connection index
 *
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SM_FreeActConnIdx
(
  FrArTp_ActConnIdxType actConnIdx
);

/**
 * \brief find and returns an allocated active connection or allocates a new one if none is existing
 *
 * \param[in] connIdx     connection index
 *
 * \return FRARTP_ACTCONN_INVALID       No active connection and no ressources available for a new
 *                                      active connection
 * \return *                            active connection index for the connection
 */
TS_MOD_PRIV_DECL FUNC(FrArTp_ActConnIdxType, FRARTP_CODE) FrArTp_SM_FindOrAllocateActConnIdx
(
  FrArTp_ConnIdxType connIdx
);

/**
 * \brief change the receive connection parameters stMin or BS
 *
 * \param[in] connIdx     connection index
 * \param[in] parameter   Parameter to change
 * \param[in] value       New value of parameter
 *
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType,FRARTP_CODE) FrArTp_SM_changeParameter
(
  FrArTp_ConnIdxType connIdx,
  VAR(TPParameterType,AUTOMATIC) parameter,
  VAR(uint8,AUTOMATIC) value
);


/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>


#endif /* ifndef FRARTP_SM_INT_H */
/*==================[end of file]===========================================*/
