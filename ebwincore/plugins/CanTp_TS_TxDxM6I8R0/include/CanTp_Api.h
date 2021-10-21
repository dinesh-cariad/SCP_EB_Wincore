/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_API_H
#define CANTP_API_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Version.h>     /* CanTp version declarations */
#include <CanTp_Cfg.h>         /* CanTp configuration */
#include <CanTp_Types_Int.h>   /* CanTp Postbuild types */
#if(CANTP_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* CANTP_PBCFGM_SUPPORT_ENABLED */

/*==================[macros]================================================*/

#if (defined CANTP_PDU_DIR_TRANSMIT) /* to prevent double declaration */
#error CANTP_PDU_DIR_TRANSMIT is already defined
#endif /* if (defined CANTP_PDU_DIR_TRANSMIT) */
/** \brief Define for CanTp N-SDU direction (Tx N_SDU). */
#define CANTP_PDU_DIR_TRANSMIT 0U


#if (defined CANTP_PDU_DIR_RECEIVE) /* to prevent double declaration */
#error CANTP_PDU_DIR_RECEIVE is already defined
#endif /* if (defined CANTP_PDU_DIR_RECEIVE) */
/** \brief Define for CanTp N-SDU direction (Rx N_SDU). */
#define CANTP_PDU_DIR_RECEIVE  1U
/*==================[type definitions]======================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#if CANTP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANTP_START_SEC_CODE_CC_BLOCK
#else
#define CANTP_START_SEC_CODE
#endif
#include <CanTp_MemMap.h>

/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_IsValidConfig(
    P2CONST(void, AUTOMATIC, CANTP_APPL_CONST) voidConfigPtr);

#if CANTP_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANTP_STOP_SEC_CODE_CC_BLOCK
#include <CanTp_MemMap.h>

#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif


/** \brief Initialize the CanTP module.
 **
 ** This function initializes the CanTP module.
 **
 ** \param[in] CfgPtr Pointer to the CanTp post-build configuration data. This
 **                   parameter is ignored because the CanTp does not support
 **                   post-build configuration. Please use NULL_PTR as
 **                   parameter for the initialization.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_Init
(
  P2CONST(CanTp_ConfigType, AUTOMATIC, CANTP_APPL_CONST) CfgPtr
);


#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/**\brief Main function of the CanTp.
 **
 ** This function is the main function for scheduling CanTP.
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_MainFunction(void);


/** \brief Transfer segmented data.
 **
 ** This service is used to request the transfer of segmented data.
 **
 ** \param[in] CanTpTxSduId Contains the unique CanTp module identifier
 **                   of the CAN N-SDU to be transmitted.
 **                   Range: - 0..(maximum number of L-PDU IDs received ) - 1.
 ** \param[in] CanTpTxInfoPtr A pointer to a structure with CAN N-SDU
 **                   related data (CAN N-SDU buffer and the length of this buffer)
 **
 ** \return Std_ReturnType
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK The request cannot be started (e.g. a transmit request
 **               is in progress with the same N-SDU identifier).
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_Transmit
(
  PduIdType CanTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpTxInfoPtr
);

#if(CANTP_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the CanTP module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Module Id
 **  - Vendor Id
 **  - Vendor specific version numbers
 **
 ** \param[out] VersionInfoPtr Pointer to where to store the version
 **                   information of this module.
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANTP_APPL_DATA) VersionInfoPtr
);

#endif /* CANTP_VERSION_INFO_API == STD_ON */


#if(CANTP_TX_CANCELLATION_API == STD_ON)

/** \brief Cancel a pending transfer
 **
 ** This API-Service is used to cancel the transfer of pending Can N-SDUs.
 **
 ** \param[in] CanTpTxSduId  Tx N-SDU ID
 **
 ** \return Std_ReturnType
 ** \retval E_OK cancellation request accepted
 ** \retval E_NOT_OK cancellation request rejected
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CancelTransmit
(
  PduIdType CanTpTxSduId
);

#endif /* CANTP_TX_CANCELLATION_API */


#if(CANTP_DYNAMIC_NSA_API == STD_ON)

/** \brief Set N_SA value for a specific N-SDU ID.
 **
 ** This service sets the N_SA value for the given N-SDU if it is configured
 ** with extended addressing format. N_SA in this context always refers to the
 ** address of this (the own) ECU, independent of the N-SDU direction.
 **
 ** For CanTpDirection CANTP_PDU_DIR_RECEIVE it can be found in CanTp configuration
 ** in the configuration parameter CanTpRxNSdu/CanTpRxNSduId.
 **
 ** For CanTpDirection CANTP_PDU_DIR_TRANSMIT it can be found in CanTp configuration
 ** in the configuration parameter CanTpTxNSdu/CanTpTxNSduId.
 **
 ** \param[in] CanTpPduId      Contains the N-SDU ID
 ** \param[in] CanTpDirection  CANTP_PDU_DIR_TRANSMIT and CANTP_PDU_DIR_RECEIVE
 ** \param[in] CanTpNSa        N_SA value to be set
 **
 ** \return Std_ReturnType
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK  Det error occured or N-SDU is not configured.
 **
 ** \ServiceID{0x1E}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_SetNSa
(
  PduIdType CanTpPduId,
  uint8     CanTpDirection,
  uint8     CanTpNSa
);
#endif /* CANTP_DYNAMIC_NSA_API */


#if(CANTP_DYNAMIC_NSA_API == STD_ON)

/** \brief Get N_SA value for a specific N-SDU ID.
 **
 ** This service gets the N_SA value for the given N-SDU if it is configured
 ** with extended addressing format. N_SA in this context always refers to the
 ** address of this (the own) ECU, independent of the N-SDU direction.
 **
 ** For CanTpDirection CANTP_PDU_DIR_RECEIVE it can be found in CanTp configuration
 ** in the configuration parameter CanTpRxNSdu/CanTpRxNSduId.
 **
 ** For CanTpDirection CANTP_PDU_DIR_TRANSMIT it can be found in CanTp configuration
 ** in the configuration parameter CanTpTxNSdu/CanTpTxNSduId.
 **
 ** \param[in] CanTpPduId       Contains the N-SDU ID
 ** \param[in] CanTpDirection   CANTP_PDU_DIR_TRANSMIT and CANTP_PDU_DIR_RECEIVE
 ** \param[out] CanTpNSaPtr     A pointer to a N_SA value, where the current N_SA value
 **                             can be written to.
 ** \return Std_ReturnType
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Det error occured or N-SDU is not configured.
 **
 ** \ServiceID{0x1F}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetNSa
(
  PduIdType CanTpPduId,
  uint8     CanTpDirection,
  P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) CanTpNSaPtr
);
#endif /* CANTP_DYNAMIC_NSA_API */

#if(CANTP_CHANGE_PARAMETER_REQ_API == STD_ON)

/** \brief Change parameter BS or STmin
 **
 ** This API-Service is used to request the change of reception parameters
 ** BS and STmin for a specified N-SDU.
 **
 ** \param[in] Id  Identifier of the received N-SDU on which the parameter has to be changed.
 ** \param[in] Parameter  Specify the parameter to which the value has to be changed (BS or STmin).
 ** \param[in] Value  The new value of the parameter.
 **
 ** Preconditions:
 ** - Related N-SDU must not be in the process of reception
 ** - Function parameter must be valid
 **
 ** \return Std_ReturnType
 ** \retval E_OK request is accepted.
 ** \retval E_NOT_OK request is not accepted.
 **
 ** \ServiceID{0x0A}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
);

/** \brief Change parameter BS or STmin
 **
 ** This API-Service is used to request the change of reception parameters
 ** BS and STmin for a specified N-SDU.
 **
 ** \param[in] Id  Identifier of the received N-SDU on which the parameter has to be changed.
 ** \param[in] Parameter  Specify the parameter to which the value has to be changed (BS or STmin).
 ** \param[in] Value  The new value of the parameter.
 **
 ** Preconditions:
 ** - Related N-SDU must not be in the process of reception
 ** - Function parameter must be valid
 **
 ** \return Std_ReturnType
 ** \retval E_OK request is accepted.
 ** \retval E_NOT_OK request is not accepted.
 **
 ** \ServiceID{0x0A}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeRxParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
);

#endif /* CANTP_CHANGE_PARAMETER_REQ_API == STD_ON */

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
/** \brief Change parameter STmin
 **
 ** This API-Service is used to request the change of transmission parameter
 ** STmin for a specified N-SDU.
 **
 ** \param[in] Id  Identifier of the transmitted N-SDU on which the parameter has to be changed.
 ** \param[in] Parameter  Specify the parameter to which the value has to be changed (STmin).
 ** \param[in] Value  The new value of the parameter.
 **
 ** Preconditions:
 ** - Function parameter must be valid
 **
 ** \return Std_ReturnType
 ** \retval E_OK request is accepted.
 ** \retval E_NOT_OK request is not accepted.
 **
 ** \ServiceID{0x0C}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_ChangeTxParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
);

/** \brief Reset parameter STmin
 **
 ** This API-Service is used to request the reset of transmission parameter
 ** STmin for a specified N-SDU.
 **
 ** \param[in] Id  Identifier of the transmitted N-SDU on which the parameter has to be reset.
 **
 ** Preconditions:
 ** - Function parameter must be valid
 **
 ** \return Std_ReturnType
 ** \retval E_OK request is accepted.
 ** \retval E_NOT_OK request is not accepted.
 **
 ** \ServiceID{0x0D}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTP_CODE) CanTp_ResetTxParameter
(
  PduIdType Id
);

#endif /* CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON */


#if(CANTP_READ_PARAMETER_REQ_API == STD_ON)

/** \brief Read parameter BS or STmin
 **
 ** This API-Service is used to read reception parameters
 ** BS and STmin for a specified N-SDU.
 **
 ** \param[in] id  Identifier of the received N-SDU on which the reception parameters are read.
 ** \param[in] parameter  Specify the parameter to which the value has to be read (BS or STmin).
 ** \param[in] value  Pointer where the parameter value will be provided.
 **
 ** Preconditions:
 ** - Function parameter must be valid
 **
 ** \return Std_ReturnType
 ** \retval E_OK request is accepted.
 ** \retval E_NOT_OK request is not accepted.
 **
 ** \ServiceID{0x0B}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_ReadParameter
(
  PduIdType id,
  TPParameterType parameter,
  uint16* value
);
#endif /* CANTP_READ_PARAMETER_REQ_API == STD_ON */

#if(CANTP_RX_CANCELLATION_API == STD_ON)

/** \brief Cancel an ongoing reception
 **
 ** This service is used to cancel an ongoing segmented reception.
 **
 ** ** Preconditions:
 ** - Related N-SDU is segmented and not waiting for the last CF.
 ** - N-SDU id must be valid.
 ** - Module must be initialized.
 ** - Corresponding channel is not waiting for a response of the lower layer.
 **
 ** \param[in] CanTpRxSduId  Identifier of the received N-SDU.
 **
 ** \return Std_ReturnType
 ** \retval E_OK Cancellation request of the specified N-SDU is accepted.
 ** \retval E_NOT_OK Cancellation request is rejected; the reason can be that request is issued for
 **                  an N-SDU that is not segmented or request is issued for an N-SDU that is not
 **                  in the reception process.
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANTP_CODE) CanTp_CancelReceive
(
  PduIdType CanTpRxSduId
);
#endif /* CANTP_RX_CANCELLATION_API == STD_ON */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

#endif /* ifndef CANTP_API_H */
/*==================[end of file]===========================================*/
