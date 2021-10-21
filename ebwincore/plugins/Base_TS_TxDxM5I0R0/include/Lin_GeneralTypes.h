#ifndef LIN_GENERALTYPES_H
#define LIN_GENERALTYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>        /* AUTOSAR COM stack type definitions */
#include <Lin_GeneralTypes_Cfg.h>  /* configuration dependent LIN type definitions */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/* *** LIN Driver types *** */

/** \brief Represents all valid protected identifier used by Lin_SendFrame().
 *
 * The LIN identifier (0..0x3F) together with its two parity bits. */
typedef uint8 Lin_FramePidType;

/** \brief This type is used to specify the Checksum model to be used for the
 * LIN Frame. */
typedef enum
{
  LIN_ENHANCED_CS, /**< Enhanced checksum model */
  LIN_CLASSIC_CS   /**< Classic checksum model */
} Lin_FrameCsModelType;

/** \brief This type is used to specify the number of SDU data bytes to
 * copy. */
typedef uint8 Lin_FrameDlType;

/** \brief This Type is used to provide PID, checksum model, data length and
 * SDU pointer from the LIN Interface to the LIN driver. */
typedef struct
{
  Lin_FramePidType                            Pid;
  Lin_FrameCsModelType                        Cs;
  Lin_FrameResponseType                       Drc;
  Lin_FrameDlType                             Dl;
  P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) SduPtr; /**< Pointer to the data */
} Lin_PduType;


/** \brief LIN operation states for a LIN channel or frame, as returned by the
 * API service Lin_GetStatus(). */
typedef enum
{
  LIN_NOT_OK = 0,      /**< LIN frame operation return value. Development or
                        * production error occurred */
  LIN_TX_OK,           /**< LIN frame operation return value. Successful
                        * transmission. */
  LIN_TX_BUSY,         /**< LIN frame operation return value. Ongoing
                        * transmission (Header or Response). */
  LIN_TX_HEADER_ERROR, /**< LIN frame operation return value. Erroneous header
                        * transmission such as:
                        * - Mismatch between sent and read back data
                        * - Identifier parity error or
                        * - Physical bus error */
  LIN_TX_ERROR,        /**< LIN frame operation return value. Erroneous
                        * response transmission such as:
                        * - Mismatch between sent and read back data
                        * - Physical bus error */
  LIN_RX_OK,           /**< LIN frame operation return value. Reception of
                        * correct response. */
  LIN_RX_BUSY,         /**< LIN frame operation return value. Ongoing
                        * reception: at least one response byte has been
                        * received, but the checksum byte has not been
                        * received. */
  LIN_RX_ERROR,        /**< LIN frame operation return value. Erroneous
                        * response reception such as:
                        * - Framing error
                        * - Overrun error
                        * - Checksum error or
                        * - Short response */
  LIN_RX_NO_RESPONSE,  /**< LIN frame operation return value. No response byte
                        * has been received so far. */
  LIN_OPERATIONAL,     /**< LIN channel state return value. Normal operation;
                        * the related LIN channel is ready to transmit next
                        * header. No data from previous frame available
                        * (e.g. after initialization). */
  LIN_CH_SLEEP         /**< LIN channel state return value. Sleep state
                        * operation; in this state wake-up detection from
                        * slave nodes is enabled. */
} Lin_StatusType;

/** \brief This type represents the slave error types that are detected during header reception
 * and response transmission / reception. */
typedef enum
{
  LIN_ERR_HEADER = 0,   /**< Error in header */
  LIN_ERR_RESP_STOPBIT, /**< Framing error in response */
  LIN_ERR_RESP_CHKSUM,  /**< Checksum error */
  LIN_ERR_RESP_DATABIT, /**<  Monitoring error of transmitted data bit in response */
  LIN_ERR_NO_RESP,      /**< No response */
  LIN_ERR_INC_RESP      /**< Incomplete response */
} Lin_SlaveErrorType;

/* *** LIN Transceiver Driver types *** */

/** \brief Operating modes of the LIN Transceiver Driver. */
typedef enum
{
  LINTRCV_TRCV_MODE_NORMAL = 0,  /**< Transceiver mode NORMAL */
  LINTRCV_TRCV_MODE_SLEEP,       /**< Transceiver mode SLEEP */
  LINTRCV_TRCV_MODE_STANDBY      /**< Transceiver mode STANDBY */
} LinTrcv_TrcvModeType;


/** \brief This type shall be used to control the LIN transceiver concerning
 *         wake up events and wake up notifications. */
typedef enum
{
  LINTRCV_WUMODE_ENABLE = 0,    /**< The notification for wakeup events is enabled on the addressed
                                     network. */
  LINTRCV_WUMODE_DISABLE,       /**< The notification for wakeup events is disabled on the
                                     addressed network. */
  LINTRCV_WUMODE_CLEAR          /**< A stored wakeup event is cleared on the addressed network. */
} LinTrcv_TrcvWakeupModeType;


/** \brief This type denotes the wake up reason detected by the LIN transceiver
 *         in detail. */
typedef enum
{
  LINTRCV_WU_ERROR = 0,         /**< Due to an error wake up reason was not detected. This value may
                                     only be reported when error was reported to DEM before. */
  LINTRCV_WU_NOT_SUPPORTED,     /**< The transceiver does not support any information for the wake
                                     up reason. */
  LINTRCV_WU_BY_BUS,            /**< The transceiver has detected, that the network has caused the
                                     wake up of the ECU. */
  LINTRCV_WU_BY_PIN,            /**< The transceiver has detected a wake-up event at one of the
                                     transceiver's pins (not at the LIN bus). */
  LINTRCV_WU_INTERNALLY,        /**< The transceiver has detected, that the network has woken up by
                                     the ECU via a request to NORMAL mode. */
  LINTRCV_WU_RESET,             /**< The transceiver has detected, that the "wake up" is due to an
                                     ECU reset. */
  LINTRCV_WU_POWER_ON           /**< The transceiver has detected, that the "wake up" is due to an
                                     ECU reset after power on. */
} LinTrcv_TrcvWakeupReasonType;

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
#endif /* ifndef LIN_GENERALTYPES_H */
