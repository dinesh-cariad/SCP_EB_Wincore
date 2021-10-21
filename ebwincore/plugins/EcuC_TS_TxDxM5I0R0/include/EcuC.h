/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ECUC_H
#define ECUC_H

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>            /* Com Stack Types */
#include <EcuC_Types.h>
#include <EcuC_Cfg.h>
#include <EcuC_Version.h>
/*==================[macros]================================================*/

/** \brief Interface compatibility check macro for EcuC library user
 *
 * \param[in] ecuc_sw_major_version - required EcuC AUTOSAR module major version
 * \param[in] ecuc_sw_minor_version - required EcuC AUTOSAR module minor version
 * \param[in] ecuc_sw_patch_version - at least required EcuC AUTOSAR module patch version
 *
 * \return Function execution success status
 * \retval TRUE The versions are compatible with the EcuC library
 * \retval FALSE The versions are not compatible with the EcuC library
 */
#define ECUC_COMPATIBILITY_VERSION_CHECK(ecuc_sw_major_version, ecuc_sw_minor_version, ecuc_sw_patch_version)   \
    ((ECUC_SW_MAJOR_VERSION == (ecuc_sw_major_version)) &&             \
     (ECUC_SW_MINOR_VERSION == (ecuc_sw_minor_version)) &&             \
     (ECUC_SW_PATCH_VERSION >= (ecuc_sw_patch_version)))

/** \brief  Ethernet MAC address meta data item type */
#define ECUC_METADATA_ITEM_ETHERNET_MAC_64                                0U
/** \brief  CAN ID according to ISO 11898-2 meta data item type */
#define ECUC_METADATA_ITEM_CAN_ID_32                                      1U
/** \brief SoAd socket connection ID meta data item type */
#define ECUC_METADATA_ITEM_SOCKET_CONNECTION_ID_16                        2U
/** \brief  Source address of CanTp, FrTp, or DoIP meta data item type */
#define ECUC_METADATA_ITEM_SOURCE_ADDRESS_16                              3U
/** \brief  Target address of CanTp, FrTp, or DoIP data item type */
#define ECUC_METADATA_ITEM_TARGET_ADDRESS_16                              4U
/** \brief  Address extension field CAN ID of ISO 15765-2 meta data item type */
#define ECUC_METADATA_ITEM_ADDRESS_EXTENSION_8                            5U
/** \brief  LIN node address meta data item type */
#define ECUC_METADATA_ITEM_LIN_NAD_8                                      6U
/** \brief  Priority field of SAE J1939 IDs, or Ethernet QoS  meta data item type */
#define ECUC_METADATA_ITEM_PRIORITY_8                                     7U
/** \brief DoIP payload meta data item type */
#define ECUC_METADATA_ITEM_PAYLOAD_TYPE_16                                8U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ECUC_START_SEC_CODE
#include <EcuC_MemMap.h>

/** \brief Initialize RxInit
 *
 * This function Initializes the control data structure of the receive buffer,
 *   e.g. state of the receive buffer.
 *
 * \param[in] RxControlInfoPtr control information of the passed TP-receive buffer
 */
extern FUNC(void, ECUC_CODE) EcuC_InitRx
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
);

/** \brief Start of reception
 *
 * This function returns the pointer to the size of the internal receive buffer
 *
 * \param[in] RxControlInfoPtr  control information of the passed TP-receive buffer
 * \param[in] RxBufferInfoPtr   pointer to the beginning of the TP-receive buffer and size of the buffer
 * \param[in] TpSduLength       Complete length of the TP I-PDU to be received
 * \param[out] RxBufferSizePtr  Pointer to the size of internal TP-receive buffer
 *
 * \return Function execution success status
 * \retval BUFREQ_OK:       Connection has been accepted.
 * \retval BUFREQ_E_NOT_OK: Connection has been rejected
 * \retval BUFREQ_E_OVFL:   In case the receive buffer size is smaller than TpSduLength
 */
extern FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_StartOfReception
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) RxBufferInfoPtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
);

/** \brief copy received data
 *
 * This function copies received data to the receive TP buffer
 *
 * \param[in] RxControlInfoPtr  control information of the passed TP-receive buffer
 * \param[in] RxBufferInfoPtr   pointer to the beginning of the TP-receive buffer and size of the buffer
 * \param[in] PduInfoPtr        Pointer to a PduInfoType which indicates the number of
 *                              bytes to be copied and the location of the source data
 * \param[out] RxBufferSizePtr  Remaining receive buffer after completion of this call
 *
 * \return Function execution success status
 * \retval BUFREQ_OK:       Data has been copied
 * \retval BUFREQ_E_NOT_OK: Request failed
 */
extern FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_CopyRxData
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) RxBufferInfoPtr,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
);

/** \brief current status of reception
 *
 * This function Indicates if reception is currently ongoing or received data is stopped
 *
 * \param[in] RxControlInfoPtr  control information of the passed TP-receive buffer
 *
 * \return Function execution success status
 * \retval TRUE:  Reception is currently ongoing
 * \retval FALSE: Reception finished
 */
extern FUNC(boolean, ECUC_CODE) EcuC_RxBufferIsLocked
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr
);

/** \brief perform parameter checks
 *
 * This function performs parameter checks for \<module\>_CopyRxData
 *
 * \param[in] PduInfoPtr       Pointer to a PduInfoType which indicates the number of
 *                             bytes to be copied and the location of the source data
 * \param[out] RxBufferSizePtr Assigned Rx buffer
 *
 * \return Function execution success status
 * \retval E_OK:     parameters are valid
 * \retval E_NOT_OK: parameter contains an unacceptable value
 */
extern FUNC(Std_ReturnType, ECUC_CODE) EcuC_CopyRxDataDet
(
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
);

/** \brief perform parameter checks
 *
 * This function performs parameter checks for \<module\>_StartOfReception
 *
 * \param[in] TpSduLength  Complete length of the TP I-PDU to be received
 * \param[out] RxBufferSizePtr Assigned Rx buffer
 *
 * \return Function execution success status
 * \retval E_OK:     parameters are valid
 * \retval E_NOT_OK: parameter contains an unacceptable value
 */
extern FUNC(Std_ReturnType, ECUC_CODE) EcuC_StartOfReceptionDet
(
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) RxBufferSizePtr
);

/** \brief indicates completion of reception
 *
 * This function indicates that the reception has been completed
 *
 * \param[in] RxControlInfoPtr  control information of the passed TP-receive buffer
 * \param[in] Result            Result of PDU reception
 * \param[out] CopiedDataSize    copied message length
 *
 * \return Function execution success status
 * \retval E_OK:     message successfully received
 * \retval E_NOT_OK: error occurred during reception
 */
extern FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpRxIndication
(
  P2VAR(EcuC_RxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) RxControlInfoPtr,
  NotifResultType Result,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) CopiedDataSize
);


/** \brief Initalize transmit buffer
 *
 * Initializes the control data structure of the transmit buffer, e.g. state of the transmit
 * buffer.
 *
 * \param[in] TxControlInfoPtr  Contains the control information of the passed TPtransmit buffer
 */
extern FUNC(void, ECUC_CODE) EcuC_InitTx
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
);


/** \brief Copies the requested transmit data from TxBufferInfoPtr to PduInfoPtr
 *
 * At invocation of EcuC_CopyTxData the function copies the requested transmit data from
 * TxBufferInfoPtr to the location specified by the PduInfoPtr. The function EcuC_CopyTxData also
 * calculates and sets the TxDataCntPtr to the amount of remaining bytes for the transmission of
 * this large I-PDU.
 *
 * \param[in]  TxControlInfoPtr  Contains the control information of the passed TPtransmit buffer
 * \param[in]  TxBufferInfoPtr   Contains pointer to the beginning of the TP-transmit buffer and
 *                               size of the buffer
 * \param[in]  PduInfoPtr        Pointer to a PduInfoType, which indicates the number of bytes to
 *                               be copied (SduLength) and the location where the data have to be
 *                               copied to (SduDataPtr). An SduLength of 0 is possible in order to
 *                               poll the available transmit data count. In this case no data are
 *                               to be copied and SduDataPtr might be invalid.
 * \param[in]  RetryInfoPtr      TP_DATARETRY indicates that this API call shall
 *                               copy previously copied data in order to recover from
 *                               an error. In this case TxTpDataCnt specifies the
 *                               offset in bytes from the current data copy position.
 *                               NULL_PTR or !TP_DATARETRY copy the remaining
 *                               data which has not been copied yet
 * \param[out] TxDataCntPtr      Contains the length of the remaining data that needs to be copied
 *
 * \return Result of operation
 * \retval BUFREQ_OK        Data has been copied to the transmit buffer completely as
 *                               requested.
 * \retval BUFREQ_E_NOT_OK  Data has not been copied. Request failed, in case the
 *                               corresponding I-PDU was stopped
 * \retval BUFREQ_E_BUSY    if number of bytes to copy is greater than remaining length
 */
extern FUNC(BufReq_ReturnType, ECUC_CODE) EcuC_CopyTxData
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_CONST) TxBufferInfoPtr,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, ECUC_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, ECUC_APPL_DATA) TxDataCntPtr
);


/** \brief Indicates the state of the transmission
 *
 * Indicates if transmission is currently ongoing (buffer is locked) or transmission is
 * stopped (Idle).
 *
 * \param[in]  TxControlInfoPtr  Contains the control information of the passed TPtransmit buffer
 *
 * \return Result of operation
 * \retval TRUE             Transmission is currently ongoing.
 * \retval FALSE            Transmission is finished
 */
extern FUNC(boolean, ECUC_CODE) EcuC_TxBufferIsLocked
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr
);


/** \brief Transmission of a TP message
 *
 * This function triggers the transmission of a TP message.
 *
 * \param[in]  TxControlInfoPtr  Contains the control information of the passed TPtransmit buffer
 * \param[in]  PduId             Identification of the I-PDU.
 * \param[in]  TxBufferInfoPtr   Length and pointer to the buffer of the I-PDU
 * \param[in]  LoTpTransmitFunctPtr  Transmit function of the lower layer e.g. PduR_ComTransmit
 *
 * \return Result of operation
 * \retval E_OK             Request is accepted
 * \retval E_NOT_OK         Request is not accepted e.g. transmission currently ongoing
 */
extern FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpTransmit
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_CONST) TxBufferInfoPtr,
  EcuC_LoTpTransmitFunctPtrType LoTpTransmitFunctPtr
);


/** \brief Indication that the transmission has been completed
 *
 * EcuC_TpTxConfirmation is called to indicate that the transmission has been completed.
 *
 * \param[in]  TxControlInfoPtr  Contains the control information of the passed TPtransmit buffer
 * \param[in]  Result            NTFRSLT_OK: the complete I-PDU has been transmitted
 *                               \<ANY OTHER VALUE\>: the I-PDU has not been transmitted, the
 *                               transmit buffer can be unlocked
 *
 * \return Result of operation
 * \retval E_OK             Message successfully transmitted
 * \retval E_NOT_OK         Error occured during transmission, data was not fully transmitted
 */
extern FUNC(Std_ReturnType, ECUC_CODE) EcuC_TpTxConfirmation
(
  P2VAR(EcuC_TxControlInfoType, AUTOMATIC, ECUC_APPL_DATA) TxControlInfoPtr,
  NotifResultType Result
);

#if (ECUC_META_DATA_HANDLING_ENABLED == STD_ON)

/** \brief Set address extension meta data
 *
 * Sets the address extension meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  AddrExtension   addrExtension
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         addrExtension not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataAddrExtension
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 AddrExtension
);

/** \brief Set Can id meta data item
 *
 * Sets the Can id meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  CanId           CAN ID
 *
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         canId not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataCanId
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint32 CanId
);


/** \brief Set Ethernet Mac Address meta data
 *
 * Sets the Ethernet Mac Address meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  EthMacPtr       Ethernet Mac Address
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         ethMac not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataEthMac
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2CONST(uint8, AUTOMATIC, ECUC_APPL_DATA) EthMacPtr
);

/** \brief Set Lin node address meta data item
 *
 * Sets the Lin node address meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  LinNad          LIN node address as used in the
 *                             LIN transport protocol
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         linNad not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataLinNad
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 LinNad
);


/** \brief Sets priority field meta data item
 *
 * Sets the priority field meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  Priority        Priority field of SAEl
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         priority not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPriority
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 Priority
);


/** \brief Set SoAd socket connection ID meta data item
 *
 * Sets the SoAd socket connection ID meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  SoConId         SoAd socket connection ID
 *
 * \return Result of operation
 * \retval E_OK           value has been saved
 * \retval E_NOT_OK       SoConId not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataSoConId
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 SoConId
);

/** \brief Set source address meta data item
 *
 * Sets the source address meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  SrcAddr         Source address of CanTp, FrTp, or DoIP transport
                               protocol messages, or of SAE J1939 messages
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         srcAddr not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataSourceAddr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 SrcAddr
);

/** \brief Set target address meta data item
 *
 * Sets the target address meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  TargetAddr      Target address of CanTp, FrTp, or DoIP transport
                               protocol messages, or destination address of SAE
                               J1939 messages
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         TargetAddr not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataTargetAddr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 TargetAddr
);

/** \brief Set payload type meta data item
 *
 * Sets the payload type meta data item of the referenced PDU in the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out] PduInfoPtr      pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[in]  PldType         payload type
 *
 * \return Result of operation
 * \retval E_OK             value has been saved
 * \retval E_NOT_OK         PldType not configured for this PDU
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPayloadType
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 PldType
);

/** \brief Retrieve address extension meta data item
 *
 * Retrieves the address extension meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]   PduId          ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  AddrExtension  pointer to stored address extension field
 *
 * \return Result of operation
 * \retval E_OK               value has been copied to addrExtension
 * \retval E_NOT_OK           addrExtension not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataAddrExtension
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) AddrExtension

);

/** \brief Retrieve Can Id meta data item
 *
 * Retrieves the Can Id meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  CanId          pointer to stored CAN ID according to ISO 11898-2
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to canId
 * \retval E_NOT_OK            canId not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataCanId
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, ECUC_APPL_DATA) CanId
);

/** \brief Retrieve Ethernet Mac Address meta data item
 *
 * Retrieves the Ethernet Mac Address meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  EthMacPtr      pointer to the Ethernet MAC address
 *
 * \return Result of operation
 * \retval E_OK                values has been copied to ethMacPtr
 * \retval E_NOT_OK            Ethernet MAC not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataEthMac
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) EthMacPtr
);

/** \brief Retrieve Lin node address meta data item
 *
 * Retrieves the Lin node address meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  LinNad         LIN node address as used in the LIN transport protocol
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to linNad
 * \retval E_NOT_OK            lin Nad not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataLinNad
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) LinNad
);


/** \brief Retrieve priority field meta data item
 *
 * Retrieves the priority field meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  Priority       Priority field of SAE J1939 IDs
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to priority
 * \retval E_NOT_OK            priority field not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPriority
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) Priority
);


/** \brief Retrieve SoAd socket connection id meta data item
 *
 * Retrieves the SoAd socket connection id meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  SoConId     SoAd socket connection ID
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to SoConId
 * \retval E_NOT_OK            SoAd socket connection ID not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataSoConId
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) SoConId
);


/** \brief Retrieve source address meta data item
 *
 * Retrieves the source address meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]  PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  SrcAddr        Source address of CanTp, FrTp, or DoIP transport
 *                             protocol messages, or of SAE J1939 messages
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to srcAddr
 * \retval E_NOT_OK            Source Address not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataSourceAddr
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) SrcAddr
);

/** \brief Retrieve target address meta data item
 *
 * Retrieves the target address meta data item of the referenced PDU from the meta data memory
 *
 * \param[in]   PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  TargetAddr     Target address of CanTp, FrTp, or DoIP transport
 *                             protocol messages, or destination address of SAE
 *                             J1939 messages
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to TargetAddr
 * \retval E_NOT_OK            Target address not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataTargetAddr
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) TargetAddr
);

/** \brief Retrieve payload type meta data item
 *
 * Retrieves the payload type data item of the referenced PDU from the meta data memory
 *
 * \param[in]   PduId           ID of the PDU
 * \param[out]  PduInfoPtr     pointer to the PDU info including
 *                             the pointer to the meta data (currently not used)
 * \param[out]  PldType        payload type
 *
 * \return Result of operation
 * \retval E_OK                value has been copied to PldType
 * \retval E_NOT_OK            payload type not configured for this PDU.
 */
extern FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPayloadType
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) PldType
);

#endif /* (ECUC_META_DATA_HANDLING_ENABLED == STD_ON) */

#define ECUC_STOP_SEC_CODE
#include <EcuC_MemMap.h>

/*==================[end of file]===========================================*/
#endif /* ifndef ECUC_H */
