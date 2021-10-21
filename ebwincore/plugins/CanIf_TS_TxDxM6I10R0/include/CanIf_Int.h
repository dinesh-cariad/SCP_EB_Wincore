/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANIF_INT_H
#define CANIF_INT_H

 /*==================[inclusions]=============================================*/

#include <CanIf_Cfg.h>          /* Generated PreCompile configuration */
#include <CanIf_Types_Int.h>    /* Generated PostBuild configuration types */
#include <CanIf.h>              /* CAN interface types */

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if( defined TS_RELOCATABLE_CFG_ENABLE)*/
#define TS_RELOCATABLE_CFG_ENABLE CANIF_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>           /* relocatable post-build macros */
/*==================[macros]=================================================*/

  /* --- CanIf instance ID ----------------------------------------------- */

/** \brief CAN interface instance id
 **
 ** Instance ID of the CAN interface.
 */
#if (defined CANIF_INSTANCE_ID)
#error CANIF_INSTANCE_ID already defined!
#endif /* #if( defined CANIF_INSTANCE_ID)*/
#define CANIF_INSTANCE_ID                   0U



  /* --- CanIf state ----------------------------------------------------- */

/** \brief CanIf is uninitialized */
#if (defined CANIF_UNINITIALIZED)
#error CANIF_UNINITIALIZED already defined!
#endif /* #if( defined CANIF_UNINITIALIZED)*/
#define CANIF_UNINITIALIZED                 0U

/** \brief CanIf is initialized */
#if (defined CANIF_INITIALIZED)
#error CANIF_INITIALIZED already defined!
#endif /* #if( defined CANIF_INITIALIZED)*/
#define CANIF_INITIALIZED                   1U



  /* --- controller flags ------------------------------------------------ */

/** \brief Mask for CAN controller wakeup flag */
#if (defined CANIF_WAKEUP_MASK)
#error CANIF_WAKEUP_MASK already defined!
#endif /* #if( defined CANIF_WAKEUP_MASK)*/
#define CANIF_WAKEUP_MASK                   0x01U

/** \brief Mask for CAN controller wakeup validation flag */
#if (defined CANIF_WAKEUP_VALIDATION_MASK)
#error CANIF_WAKEUP_VALIDATION_MASK already defined!
#endif /* #if( defined CANIF_WAKEUP_VALIDATION_MASK)*/
#define CANIF_WAKEUP_VALIDATION_MASK        0x02U

/** \brief Mask for CAN controller Tx confirmation flag */
#if (defined CANIF_TX_CONFIRMATION_MASK)
#error CANIF_TX_CONFIRMATION_MASK already defined!
#endif /* #if( defined CANIF_TX_CONFIRMATION_MASK)*/
#define CANIF_TX_CONFIRMATION_MASK          0x04U

/** \brief Mask for CAN controller partial networking filter flag */
#if (defined CANIF_PN_FILTER_ACTIVE_MASK)
#error CANIF_PN_FILTER_ACTIVE_MASK already defined!
#endif /* #if( defined CANIF_PN_FILTER_ACTIVE_MASK)*/
#define CANIF_PN_FILTER_ACTIVE_MASK         0x08U

/** \brief Mask for CAN controller mirroring flag */
#if (defined CANIF_BUS_MIRRORING_MASK)
#error CANIF_BUS_MIRRORING_MASK already defined !
#endif /* #if (defined CANIF_BUS_MIRRORING_MASK) */
#define CANIF_BUS_MIRRORING_MASK            0x10U

  /* --- software filtering ---------------------------------------------- */

/** \brief Binary software filtering used for this L-PDU */
#if (defined CANIF_BINARY_SOFTWARE_FILTERING)
#error CANIF_BINARY_SOFTWARE_FILTERING already defined!
#endif /* #if( defined CANIF_BINARY_SOFTWARE_FILTERING)*/
#define CANIF_BINARY_SOFTWARE_FILTERING     0U

/** \brief No software filtering used for this L-PDU */
#if (defined CANIF_NO_SOFTWARE_FILTERING)
#error CANIF_NO_SOFTWARE_FILTERING already defined!
#endif /* #if( defined CANIF_NO_SOFTWARE_FILTERING)*/
#define CANIF_NO_SOFTWARE_FILTERING         255U



  /* --- dynamic Tx L-PDU type ------------------------------------------- */

/** \brief Dynamic Tx L-PDU standard CAN ID */
#if (defined CANIF_STANDARD_CAN_ID)
#error CANIF_STANDARD_CAN_ID already defined!
#endif /* #if( defined CANIF_STANDARD_CAN_ID)*/
#define CANIF_STANDARD_CAN_ID               0U

/** \brief Dynamic Tx L-PDU extended CAN ID */
#if (defined CANIF_EXTENDED_CAN_ID)
#error CANIF_EXTENDED_CAN_ID already defined!
#endif /* #if( defined CANIF_EXTENDED_CAN_ID)*/
#define CANIF_EXTENDED_CAN_ID               1U


  /* --- Det Error Ids ---------------------------------------------- */

#if (defined CANIF_E_INVALID_DLC)
#error CANIF_E_INVALID_DLC already defined!
#endif /* #if( defined CANIF_E_INVALID_DLC)*/
#define CANIF_E_INVALID_DLC                 61U

#if (defined CANIF_E_NOT_SLEEP)
#error CANIF_E_NOT_SLEEP already defined!
#endif /* #if( defined CANIF_E_NOT_SLEEP)*/
#define CANIF_E_NOT_SLEEP                   71U

#if (defined CANIF_E_FULL_QUEUE)
#error CANIF_E_FULL_QUEUE already defined!
#endif /* #if( defined CANIF_E_FULL_QUEUE)*/
#define CANIF_E_FULL_QUEUE                  90U

  /* --- partial networking ---------------------------------------------- */

/** \brief Block the Tx PDU by the partial networking filter */
#if (defined CANIF_PN_FILTER_BLOCK_PDU)
#error CANIF_PN_FILTER_BLOCK_PDU already defined!
#endif /* #if( defined CANIF_PN_FILTER_BLOCK_PDU)*/
#define CANIF_PN_FILTER_BLOCK_PDU           0U

/** \brief Allow the Tx PDU by teh partial networking filter */
#if (defined CANIF_PN_FILTER_ALLOW_PDU)
#error CANIF_PN_FILTER_ALLOW_PDU already defined!
#endif /* #if( defined CANIF_PN_FILTER_ALLOW_PDU)*/
#define CANIF_PN_FILTER_ALLOW_PDU           1U


/* --- Truncating options ----------------------------------------------------- */

/** \brief Mask for the CanIfTxPduTruncation option */
#if (defined CANIF_TX_PDU_TRUNCATION_MASK)
#error CANIF_TX_PDU_TRUNCATION_MASK already defined!
#endif /* #if( defined CANIF_TX_PDU_TRUNCATION_MASK)*/
#define CANIF_TX_PDU_TRUNCATION_MASK           1U

/** \brief Mask for the CanIfTxPduTruncateToFrame option */
#if (defined CANIF_TX_PDU_TRUNCATE_TO_FRAME_MASK)
#error CANIF_TX_PDU_TRUNCATE_TO_FRAME_MASK already defined!
#endif /* #if( defined CANIF_TX_PDU_TRUNCATE_TO_FRAME_MASK)*/
#define CANIF_TX_PDU_TRUNCATE_TO_FRAME_MASK           2U


/* --- miscellaneous --------------------------------------------------- */

/** \brief User type for upper layers for which no callback function shall
 **        be called.
 */
#if (defined CANIF_NO_UPPER_LAYER_CALLBACK)
#error CANIF_NO_UPPER_LAYER_CALLBACK already defined!
#endif /* #if( defined CANIF_NO_UPPER_LAYER_CALLBACK)*/
#define CANIF_NO_UPPER_LAYER_CALLBACK       255U

/** \brief Rx buffer DLC initialization value (invalid) */
#if (defined CANIF_INVALID_DLC)
#error CANIF_INVALID_DLC already defined!
#endif /* #if( defined CANIF_INVALID_DLC)*/
#define CANIF_INVALID_DLC                   0xFFU

/** \brief Symbol to indicate no wakeup source */
#if (defined CANIF_NO_WAKEUP_SOURCE)
#error CANIF_NO_WAKEUP_SOURCE already defined!
#endif /* #if( defined CANIF_NO_WAKEUP_SOURCE)*/
#define CANIF_NO_WAKEUP_SOURCE              0U



/** \brief Invalid CAN controller ID
 **
 ** This constant defines an invalid controller index.
 */
#if (defined CANIF_INVALID_CONTROLLER_ID)
#error CANIF_INVALID_CONTROLLER_ID already defined!
#endif /* #if( defined CANIF_INVALID_CONTROLLER_ID)*/
#define CANIF_INVALID_CONTROLLER_ID           255U

/** \brief Internal index of a single CAN controller
 **
 ** This constant defines the internal controller index of a single CAN
 ** controller. As the internal indices are always zero-based and consecutive
 ** (dense) it is always 0U.
 */
#if (defined CANIF_SINGLE_CAN_CTRL_INDEX)
#error CANIF_SINGLE_CAN_CTRL_INDEX already defined!
#endif /* #if( defined CANIF_SINGLE_CAN_CTRL_INDEX)*/
#define CANIF_SINGLE_CAN_CTRL_INDEX           0U

/** \brief Internal Id of a single CAN controller within Can
 */
#if (defined CANIF_SINGLE_CAN_CTRL_ID)
#error CANIF_SINGLE_CAN_CTRL_ID already defined!
#endif /* #if( defined CANIF_SINGLE_CAN_CTRL_ID)*/
#define CANIF_SINGLE_CAN_CTRL_ID              0U

/** \brief Invalid HOH ID index
 **
 ** This value is used to mark invalid HOH IDs in the translation array.
 */
#if (defined CANIF_INVALID_HOH_ID)
#error CANIF_INVALID_HOH_ID already defined!
#endif /* #if( defined CANIF_INVALID_HOH_ID)*/
#define CANIF_INVALID_HOH_ID                  0xFFFFU

/** \brief Mask to identify HTHs
 **
 ** This value is added to the CanIf internal index of each HTH to be able to
 ** verify, that this is a HTH. This is used for DET reporting only.
 ** It is set to 0 if DET reporting is disabled.
 */
#if CANIF_DEV_ERROR_DETECT == STD_ON
#if (defined CANIF_HTH_ID_MASK)
#error CANIF_HTH_ID_MASK already defined!
#endif /* #if( defined CANIF_HTH_ID_MASK)*/
#define CANIF_HTH_ID_MASK                     0x8000U
#else /* CANIF_DEV_ERROR_DETECT */
#if (defined CANIF_HTH_ID_MASK)
#error CANIF_HTH_ID_MASK already defined!
#endif /* #if( defined CANIF_HTH_ID_MASK)*/
#define CANIF_HTH_ID_MASK                     0x0000U
#endif /* CANIF_DEV_ERROR_DETECT */

/** \brief invalid index for Tx L-PDU buffers
 **
 ** This constant defines the index used for the state "no Tx L-PDU buffer"
 ** assigned.
 */
#if (defined CANIF_INVALID_TX_BUFFER_INDEX)
#error CANIF_INVALID_TX_BUFFER_INDEX already defined!
#endif /* #if( defined CANIF_INVALID_TX_BUFFER_INDEX)*/
#define CANIF_INVALID_TX_BUFFER_INDEX         0xFFFFU

/** \brief invalid index for Rx L-PDU buffers
 **
 ** This constant defines the index used for the state "no Rx L-PDU buffer"
 ** assigned.
 */
#if (defined CANIF_INVALID_RX_BUFFER_INDEX)
#error CANIF_INVALID_RX_BUFFER_INDEX already defined!
#endif /* #if( defined CANIF_INVALID_RX_BUFFER_INDEX)*/
#define CANIF_INVALID_RX_BUFFER_INDEX         0xFFFFU

/** \brief Invalid L-PDU index constant
 **
 ** This constant defines an index value which is interpreted as "invalid
 ** index".
 */
#if (defined CANIF_INVALID_LPDU_INDEX)
#error CANIF_INVALID_LPDU_INDEX already defined!
#endif /* #if( defined CANIF_INVALID_LPDU_INDEX)*/
#define CANIF_INVALID_LPDU_INDEX              0xFFFFU


/** \brief Invalid transceiver index constant
 **
 ** This constant defines an index value which is interpreted as "invalid
 ** index".
 */
#if (defined CANIF_INVALID_TRCV_INDEX)
#error CANIF_INVALID_TRCV_INDEX already defined!
#endif /* #if( defined CANIF_INVALID_TRCV_INDEX)*/
#define CANIF_INVALID_TRCV_INDEX              0xFFU

/** \brief Invalid MainFunction index constant
 **
 ** This constant defines an index value which is interpreted as "invalid
 ** index".
 */
#if (defined CANIF_INVALID_MF_INDEX)
#error CANIF_INVALID_MF_INDEX already defined!
#endif /* #if( defined CANIF_INVALID_MF_INDEX)*/
#define CANIF_INVALID_MF_INDEX              0xFFU

/** \brief "No range configuration" index
 **
 ** This constant is used for Rx L-PDUs that have no reception range assigned.
 */
#if (defined CANIF_NO_RANGE_CONFIG)
#error CANIF_NO_RANGE_CONFIG already defined!
#endif /* #if( defined CANIF_NO_RANGE_CONFIG)*/
#define CANIF_NO_RANGE_CONFIG                 0xFFFFFFFFUL

/** \brief Invalid CanId index
 **
 ** This constant is used for Tx L-PDUs that have Metadata and no CanId configured.
 */
#if (defined CANIF_INVALID_CANID)
#error CANIF_INVALID_CANID already defined!
#endif /* #if( defined CANIF_INVALID_CANID)*/
#define CANIF_INVALID_CANID                 0xFFFFFFFFUL

/** \brief Status for frame awaiting mirroring
 **
 ** This constant is used to mark that a frame has been transmitted,
 ** it can be mirrored
 */
#if (defined CANIF_FRAME_READY_TO_MIRROR)
#error CANIF_FRAME_READY_TO_MIRROR already defined !
#endif
#define CANIF_FRAME_READY_TO_MIRROR           1U

/** \brief Status for frame that has been mirrored
 **
 ** This constant is used to mark that a frame has been mirrored
 **
 */
#if (defined CANIF_FRAME_MIRROR_DONE)
#error CANIF_FRAME_MIRROR_DONE already defined !
#endif
#define CANIF_FRAME_MIRROR_DONE               2U

/* relocatable post build elements access macros. */

#if (defined CANIF_PBCFG_ACCESS)
#error CANIF_PBCFG_ACCESS already defined!
#endif /* #if( defined CANIF_PBCFG_ACCESS)*/
#define CANIF_PBCFG_ACCESS(type, element) \
    (TS_UNCHECKEDGETCFG(CanIf_ConfigPtr, type, CANIF, CanIf_ConfigPtr->element))

#if (defined CANIF_PBCFG_ACCESS_DRIVER)
#error CANIF_PBCFG_ACCESS_DRIVER already defined!
#endif /* #if( defined CANIF_PBCFG_ACCESS_DRIVER)*/
#define CANIF_PBCFG_ACCESS_DRIVER(type, element) \
    (TS_UNCHECKEDGETCFG(CanIf_ConfigPtr, type, CANIF, CANIF_PBCFG_ACCESS(CanIf_CanDriverConfigType,driverConfigPtr)->element))

#if (defined CANIF_CFG_GET_TXLPDU_CFG)
#error CANIF_CFG_GET_TXLPDU_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_TXLPDU_CFG)*/
#define CANIF_CFG_GET_TXLPDU_CFG(Index)   \
    (CANIF_PBCFG_ACCESS(CanIf_TxLPduConfigType, txLPduConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_RXLPDU_CFG)
#error CANIF_CFG_GET_RXLPDU_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_RXLPDU_CFG)*/
#define CANIF_CFG_GET_RXLPDU_CFG(Index)   \
    (CANIF_PBCFG_ACCESS(CanIf_RxLPduConfigType, rxLPduConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_SORTEDRXLPDU_CFG)
#error CANIF_CFG_GET_SORTEDRXLPDU_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_SORTEDRXLPDU_CFG)*/
#define CANIF_CFG_GET_SORTEDRXLPDU_CFG(Index)   \
    (CANIF_PBCFG_ACCESS(CanIf_SortedRxLPduConfigType, sortedRxLPduConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_DRIVER_CFG)
#error CANIF_CFG_GET_DRIVER_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_DRIVER_CFG)*/
#define CANIF_CFG_GET_DRIVER_CFG(Index)   \
    (CANIF_PBCFG_ACCESS(CanIf_CanDriverConfigType, driverConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_TRCV_CFG)
#error CANIF_CFG_GET_TRCV_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_TRCV_CFG)*/
#define CANIF_CFG_GET_TRCV_CFG(Index)   \
    (CANIF_PBCFG_ACCESS(CanIf_CanTrcvChnlConfigType, trcvChnlConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_CTRL_CFG)
#error CANIF_CFG_GET_CTRL_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_CTRL_CFG)*/
#define CANIF_CFG_GET_CTRL_CFG(Index)   \
    (CANIF_PBCFG_ACCESS_DRIVER(CanIf_CanControllerConfigType, ctrlConfigPtr)[(Index)])

#if (CANIF_HOH_TRANSLATION_OPT == STD_OFF)
#if (defined CANIF_CFG_GET_HOHID2CANIF_IDX)
#error CANIF_CFG_GET_HOHID2CANIF_IDX already defined!
#endif /* #if( defined CANIF_CFG_GET_HOHID2CANIF_IDX)*/
#define CANIF_CFG_GET_HOHID2CANIF_IDX(Index)   \
    (CANIF_PBCFG_ACCESS_DRIVER(uint16, hohIdToCanIfIdxPtr)[(Index)])
#endif /* (CANIF_HOH_TRANSLATION_OPT == STD_OFF) */

#if (defined CANIF_CFG_GET_HTH_CFG)
#error CANIF_CFG_GET_HTH_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_HTH_CFG)*/
#define CANIF_CFG_GET_HTH_CFG(Index)   \
    (CANIF_PBCFG_ACCESS_DRIVER(CanIf_HthConfigType, hthConfigPtr)[(Index)])

#if (defined CANIF_CFG_GET_HRH_CFG)
#error CANIF_CFG_GET_HRH_CFG already defined!
#endif /* #if( defined CANIF_CFG_GET_HRH_CFG)*/
#define CANIF_CFG_GET_HRH_CFG(Index)   \
    (CANIF_PBCFG_ACCESS_DRIVER(CanIf_HrhConfigType, hrhConfigPtr)[(Index)])

  /* --- function like macros -------------------------------------------- */

/** \brief Get a notification flag
 **
 ** This macro shall be used ro a notification flag.
 **
 ** \param[in] FlagArray Pointer to the notification flag array
 ** \param[in] Index     Index of the bit to be set
 **
 ** \return 0 if flag was not set, another value otherwise
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined CANIF_GET_NOTIFICATION_FLAG)
#error CANIF_GET_NOTIFICATION_FLAG already defined!
#endif /* #if( defined CANIF_GET_NOTIFICATION_FLAG)*/
#define CANIF_GET_NOTIFICATION_FLAG( FlagArray, Index ) \
  ((FlagArray)[(Index)>>3U] & (uint8)(1U<<((Index) & 0x07U)))



/** \brief Set a notification flag
 **
 ** This macro shall be used for setting a notification flag.
 **
 ** \param[in/out] FlagArray Pointer to the notification flag array
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined CANIF_SET_NOTIFICATION_FLAG)
#error CANIF_SET_NOTIFICATION_FLAG already defined!
#endif /* #if( defined CANIF_SET_NOTIFICATION_FLAG)*/
#define CANIF_SET_NOTIFICATION_FLAG( FlagArray, Index ) \
  ((FlagArray)[(Index)>>3U] |= (uint8)(1U << ((Index) & 0x07U)))



/** \brief Clear a notification flag
 **
 ** This macro shall be used for clearing a notification flag.
 **
 ** \param[in/out] FlagArray Pointer to the notification flag array
 ** \param[in]     Index     Index of the bit to be set
 **
 ** \remarks Please note that the caller must make sure that this macro is
 **          executed in a critical section.
 */
#if (defined CANIF_CLEAR_NOTIFICATION_FLAG)
#error CANIF_CLEAR_NOTIFICATION_FLAG already defined!
#endif /* #if( defined CANIF_CLEAR_NOTIFICATION_FLAG)*/
#define CANIF_CLEAR_NOTIFICATION_FLAG( FlagArray, Index ) \
  ((FlagArray)[(Index)>>3U] &= (uint8)(~(uint8)(1U<<((Index) & 0x07U))))



/** \brief Checks the given CAN ID
 **
 ** This macro performs a check of the given CAN ID to be a correct CAN ID
 ** according to the definition of the type Can_IdType. It can be used in C
 ** conditions and evaluates to "true" if the given CAN ID is invalid.
 **
 ** \param[in] CanId CAN ID to be checked
 **
 ** \returns Result of the check
 ** \retval 0 if the CAN ID is valid
 ** \retval !=0 if the CAN ID is invalid
 */
#if (CANIF_CANIDTYPE_UINT32 == STD_ON)

#if (defined CANIF_CHECK_FOR_INVALID_CAN_ID)
#error CANIF_CHECK_FOR_INVALID_CAN_ID already defined!
#endif /* #if( defined CANIF_CHECK_FOR_INVALID_CAN_ID)*/
#define CANIF_CHECK_FOR_INVALID_CAN_ID( CanId ) \
  ( ( ((uint32)(CanId) & 0x20000000UL) != 0U ) || \
    ( ( ((uint32)(CanId) & 0x80000000UL) == 0U ) && \
      ( ((uint32)(CanId) & 0x3FFFF800UL) != 0U ) ) )

#else /* (CANIF_CANIDTYPE_UINT32 == STD_ON) */

#if (defined CANIF_CHECK_FOR_INVALID_CAN_ID)
#error CANIF_CHECK_FOR_INVALID_CAN_ID already defined!
#endif /* #if( defined CANIF_CHECK_FOR_INVALID_CAN_ID)*/
#define CANIF_CHECK_FOR_INVALID_CAN_ID( CanId ) \
      (((uint16)((CanId) & 0xB800UL)) != 0U)

#endif /* (CANIF_CANIDTYPE_UINT32 == STD_ON) */

/** \brief Get number of CAN controllers
 **
 ** This macro can be used, to read the number of CAN controllers from the
 ** driver configuration.
 */
#if CANIF_SINGLE_CAN_CTRL_OPT == STD_OFF

#if (defined CANIF_GET_NR_OF_CAN_CTRLS)
#error CANIF_GET_NR_OF_CAN_CTRLS already defined!
#endif /* #if( defined CANIF_GET_NR_OF_CAN_CTRLS)*/
#define CANIF_GET_NR_OF_CAN_CTRLS()  (CanIf_DriverConfigPtr->nrOfControllers)

#else /* CANIF_SINGLE_CAN_CTRL_OPT */

#if (defined CANIF_GET_NR_OF_CAN_CTRLS)
#error CANIF_GET_NR_OF_CAN_CTRLS already defined!
#endif /* #if( defined CANIF_GET_NR_OF_CAN_CTRLS)*/
#define CANIF_GET_NR_OF_CAN_CTRLS()  1U

#endif /* CANIF_SINGLE_CAN_CTRL_OPT */



/** \brief Get CAN controller assigned to an HOH
 **
 ** This macro is used to get the controller ID of a CAN controller assigned
 ** to an HOH.
 **
 ** \param[in] HohCfg HOH configuration (both, HRH and HTH)
 */
#if CANIF_SINGLE_CAN_CTRL_OPT == STD_OFF

#if (defined CANIF_GET_HOH_INT_CTRL_IDX)
#error CANIF_GET_HOH_INT_CTRL_IDX already defined!
#endif /* #if( defined CANIF_GET_HOH_INT_CTRL_IDX)*/
#define CANIF_GET_HOH_INT_CTRL_IDX(HohCfg)  ((HohCfg).intCtrlIdx)

#else /* CANIF_SINGLE_CAN_CTRL_OPT */

#if (defined CANIF_GET_HOH_INT_CTRL_IDX)
#error CANIF_GET_HOH_INT_CTRL_IDX already defined!
#endif /* #if( defined CANIF_GET_HOH_INT_CTRL_IDX)*/
#define CANIF_GET_HOH_INT_CTRL_IDX(HohCfg)  0U

#endif /* CANIF_SINGLE_CAN_CTRL_OPT */



/** \brief Get CanIf internal index of an HOH
 **
 ** This macro is used to translate a given HOH ID into the CanIf internal
 ** index to its HRH or HTH configuration array.
 **
 ** \param[in] HohId CAN Driver HOH ID
 **
 ** \return CanIf internal HOH index representation
 ** \retval HRH configuration array index in case of a receive object
 ** \retval HTH configuration array index + HTH mask for Tx objects
 ** \retval CANIF_INVALID_HOH_ID in case of an invalid HOH ID
 */
#if (defined CANIF_GET_INT_HOH_IDX)
#error CANIF_GET_INT_HOH_IDX already defined!
#endif /* #if( defined CANIF_GET_INT_HOH_IDX)*/
#define CANIF_GET_INT_HOH_IDX(HohId)        (CANIF_CFG_GET_HOHID2CANIF_IDX(HohId))


/** \brief Get the CAN driver index
 **
 ** This macro gets the index for the table with the function pointers
 ** used to call the CAN driver interfaces.
 ** Simply returns 0 if multiple driver support is disabled.
 */
#if( CANIF_MULTIPLE_DRV_SUPPORT == STD_ON )
#if (defined CANIF_GET_CAN_DRIVER_IDX)
#error CANIF_GET_CAN_DRIVER_IDX already defined!
#endif /* #if( defined CANIF_GET_CAN_DRIVER_IDX)*/
#define CANIF_GET_CAN_DRIVER_IDX(CanIfCtrlIdx)   \
              (CANIF_CFG_GET_CTRL_CFG(CanIfCtrlIdx).driverIndex)
#else
#if (defined CANIF_GET_CAN_DRIVER_IDX)
#error CANIF_GET_CAN_DRIVER_IDX already defined!
#endif /* #if( defined CANIF_GET_CAN_DRIVER_IDX)*/
#define CANIF_GET_CAN_DRIVER_IDX(CanIfCtrlIdx)   0u
#endif /* ( CANIF_MULTIPLE_DRV_SUPPORT == STD_ON ) */

/** \brief Translate ID configured in the CanIf to the ID used by CAN Driver
 **
 ** This macro is used to translate a CanIf controller ID into
 ** the CAN controller ID defined and used by the CAN Driver.
 */
#if (defined CANIF_GET_CANDRV_CTRL_ID)
#error CANIF_GET_CANDRV_CTRL_ID already defined!
#endif /* #if( defined CANIF_GET_CANDRV_CTRL_ID)*/
#define CANIF_GET_CANDRV_CTRL_ID(CanIfCtrlIdx)   \
              (CANIF_CFG_GET_CTRL_CFG(CanIfCtrlIdx).controllerId)


/** \brief Call the DLC error notification function
 **
 ** This macro is used to call the DLC error notification function of the
 ** given Rx PDU. It checks if one is configured and calls it in this case.
 **
 ** \param[in] RxLPduConfig Configuration container of the Rx L-PDU
 */
#if( CANIF_DLC_ERROR_NOTIF_SUPPORT == STD_ON )
#if (defined CANIF_DLC_ERROR_NOTIFICATION)
#error CANIF_DLC_ERROR_NOTIFICATION already defined!
#endif /* #if( defined CANIF_DLC_ERROR_NOTIFICATION)*/
#define CANIF_DLC_ERROR_NOTIFICATION( RxLPduConfig )                                                                                                    \
  do                                                                                                                                                    \
  {                                                                                                                                                     \
    if( ((RxLPduConfig)->userType != CANIF_NO_UPPER_LAYER_CALLBACK) && (CanIf_CbkFctPtrTbl[(RxLPduConfig)->userType].DlcErrorNotifFctPtr != NULL_PTR )) \
    {                                                                                                                                                   \
      (CanIf_CbkFctPtrTbl[(RxLPduConfig)->userType].DlcErrorNotifFctPtr)( (RxLPduConfig)->targetPduId );                                                \
    }                                                                                                                                                   \
  } while( 0 )
#else /* CANIF_DLC_ERROR_NOTIF_SUPPORT == STD_ON */
#if (defined CANIF_DLC_ERROR_NOTIFICATION)
#error CANIF_DLC_ERROR_NOTIFICATION already defined!
#endif /* #if( defined CANIF_DLC_ERROR_NOTIFICATION)*/
#define CANIF_DLC_ERROR_NOTIFICATION( RxLPduConfig )
#endif /* CANIF_DLC_ERROR_NOTIF_SUPPORT == STD_ON */


/** \brief Call the DLC check passed notification function
 **
 ** This macro is used to call the DLC check passed notification function of
 ** the given Rx PDU. It checks if one is configured and calls it in this
 ** case.
 **
 ** \param[in] RxLPduConfig Configuration container of the Rx L-PDU
 */
#if( CANIF_DLC_PASSED_NOTIF_SUPPORT == STD_ON )
#if (defined CANIF_DLC_PASSED_NOTIFICATION)
#error CANIF_DLC_PASSED_NOTIFICATION already defined!
#endif /* #if( defined CANIF_DLC_PASSED_NOTIFICATION)*/
#define CANIF_DLC_PASSED_NOTIFICATION( RxLPduConfig )                                                                                                    \
  do                                                                                                                                                     \
  {                                                                                                                                                      \
    if( ((RxLPduConfig)->userType != CANIF_NO_UPPER_LAYER_CALLBACK) && (CanIf_CbkFctPtrTbl[(RxLPduConfig)->userType].DlcPassedNotifFctPtr != NULL_PTR )) \
    {                                                                                                                                                    \
      (CanIf_CbkFctPtrTbl[(RxLPduConfig)->userType].DlcPassedNotifFctPtr)( (RxLPduConfig)->targetPduId );                                                \
    }                                                                                                                                                    \
  } while( 0 )
#else /* CANIF_DLC_PASSED_NOTIF_SUPPORT == STD_ON */
#if (defined CANIF_DLC_PASSED_NOTIFICATION)
#error CANIF_DLC_PASSED_NOTIFICATION already defined!
#endif /* #if( defined CANIF_DLC_PASSED_NOTIFICATION)*/
#define CANIF_DLC_PASSED_NOTIFICATION( RxLPduConfig )
#endif /* CANIF_DLC_PASSED_NOTIF_SUPPORT == STD_ON */

 /** \def CanIf_AtomicAssignCanId(CanId)
  **
  ** \brief Provide a macro for atomic assigning CanIDs
  **
  ** This macro is used to atomic assign CanIDs. Depending on whether they are 16 bit or 32 bit the
  ** respective TS-macro is used.
  **
  ** \param[in] to   CanId is written to in atomic fashion
  ** \param[in] from CanId is read from in tomic fashion
  */
#if (defined CanIf_AtomicAssignCanId)
#error CanIf_AtomicAssignCanId already defined!
#endif /* #if( defined CanIf_AtomicAssignCanId)*/
#define CanIf_AtomicAssignCanId(to,from) TS_AtomicAssign32((to),(from))

/* -----------------[defensive programming macros]------------------------ */




/*------------------------[Defensive programming]----------------------------*/

#if (defined CANIF_PRECONDITION_ASSERT)
#error CANIF_PRECONDITION_ASSERT is already defined
#endif

#if (defined CANIF_PRECONDITION_ASSERT_NO_EVAL)
#error CANIF_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANIF_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))
#else
#define CANIF_PRECONDITION_ASSERT(Condition, ApiId)
#define CANIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANIF_POSTCONDITION_ASSERT)
#error CANIF_POSTCONDITION_ASSERT is already defined
#endif

#if (defined CANIF_POSTCONDITION_ASSERT_NO_EVAL)
#error CANIF_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (CANIF_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))
#else
#define CANIF_POSTCONDITION_ASSERT(Condition, ApiId)
#define CANIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANIF_INVARIANT_ASSERT)
#error CANIF_INVARIANT_ASSERT is already defined
#endif

#if (defined CANIF_INVARIANT_ASSERT_NO_EVAL)
#error CANIF_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (CANIF_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define CANIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))
#else
#define CANIF_INVARIANT_ASSERT(Condition, ApiId)
#define CANIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined CANIF_STATIC_ASSERT)
# error CANIF_STATIC_ASSERT is already defined
#endif
#if (CANIF_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define CANIF_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define CANIF_STATIC_ASSERT(expr)
#endif

#if (defined CANIF_UNREACHABLE_CODE_ASSERT)
#error CANIF_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (CANIF_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define CANIF_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(CANIF_MODULE_ID, CANIF_INSTANCE_ID, (ApiId))
#else
#define CANIF_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined CANIF_INTERNAL_API_ID)
#error CANIF_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define CANIF_INTERNAL_API_ID DET_INTERNAL_API_ID



/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
/** \brief Helper function for transceiver without partial network
 **
 ** If partial network is enabled, but an underlying transceiver
 ** driver does not support partial network (disabled by config
 ** or no capability), then this function is linked to the
 ** CanTrcv function pointer table in place of the API function
 ** CanTrcv_ClearTrcvWufFlag().
 **
 ** \param[in] TransceiverId designated CAN transceiver
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ClearTrcvWufFlag_HlpNoPn
  (
    uint8 TransceiverId
  );

/** \brief Helper function for transceiver without partial network
 **
 ** If partial network is enabled, but an underlying transceiver
 ** driver does not support partial network (disabled by config
 ** or no capability), then this function is linked to the
 ** CanTrcv function pointer table in place of the API function
 ** CanTrcv_CheckWakeFlag().
 **
 ** \param[in] TransceiverId designated CAN transceiver
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckWakeFlag_HlpNoPn
  (
    uint8 TransceiverId
  );
#endif /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

#define CANIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanIf_MemMap.h>

/** \brief Configuration of CAN driver(s) function pointers
 **
 ** This array contains the function pointers for all the interfaces
 ** of the CAN driver(s).
 */
extern CONST( CanIf_CanDrvFctConfigType, CANIF_APPL_CONST )
  CanIf_CanDrvFctConfig[];

#if (CANIF_ANY_UL_CBK_SUPPORT == STD_ON)
/** \brief Configuration of upper layer callback functions
 **
 ** This array contains the function pointers for all upper layer callback
 ** functions.
 */
extern CONST( CanIf_CbkFctPtrTblType, CANIF_APPL_CONST ) CanIf_CbkFctPtrTbl[];
#endif /* CANIF_ANY_UL_CBK_SUPPORT == STD_ON */


#if (CANIF_TRCV_SUPPORT == STD_ON)
/** \brief Configuration of CanTrcv function pointers
 **
 ** This array contains the CanTrcv driver configurations meaning the
 ** function pointers for accessing the driver functionality.
 */
extern CONST( CanIf_CanTrcvConfigType, CANIF_APPL_CONST )
  CanIf_CanTrcvConfig[];


/** \brief Configuration of CanTrcv channels */
extern CONST( CanIf_CanTrcvChnlConfigType, CANIF_APPL_CONST )
  CanIf_CanTrcvChnlConfig[];
#endif /* CANIF_TRCV_SUPPORT == STD_ON */


#if (CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON)
/** \brief Configuration of Tx decoupled processing */
extern CONST( CanIf_MfTxPtrTblType, CANIF_APPL_CONST )
  CanIf_MfTxFctPtrTbl[];
#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON */


#if (CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Configuration of Rx decoupled processing */
extern CONST( CanIf_MfRxPtrTblType, CANIF_APPL_CONST )
  CanIf_MfRxFctPtrTbl[];
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */

#define CANIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_VAR_CLEARED_8
#include <CanIf_MemMap.h>

#if( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Bit-array of Tx notification flags (if enabled)
 **
 ** This array contains a Tx notification flag for each Tx L-PDU for which
 ** this feature is enabled.
 */
extern VAR( uint8, CANIF_VAR )
  CanIf_TxLPduNotifyFlags[];
#endif /* CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON */

#if( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Bit-array of Rx notification flags (if enabled)
 **
 ** This array contains a Rx notification flag for each Rx L-PDU for which
 ** this feature is enabled.
 */
extern VAR( uint8, CANIF_VAR )
  CanIf_RxLPduNotifyFlags[];
#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */

#if( CANIF_READRXPDU_DATA_API == STD_ON )
/** \brief Array of Rx buffers
 **
 ** This is an array which is used for buffering of Rx messages.
 */
extern VAR( uint8, CANIF_VAR ) CanIf_RxBuffer[];

#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */


#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
/** \brief Array of Tx buffers
 **
 ** This is an array which is used for buffering of Tx messages.
 */
extern VAR( uint8, CANIF_VAR ) CanIf_TxBufferMemory[];

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

#if( ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
     ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )  || \
     ( CANIF_PUBLIC_PN_SUPPORT == STD_ON) || \
     ( CANIF_BUS_MIRRORING_SUPPORT == STD_ON ) )

/** \brief Controller flags
 **
 ** This array contains the wakeup flags of all configured
 ** controllers. If a bit/flag has the value 1 it is set.
 ** Bit masks are:
 ** CANIF_WAKEUP_MASK             Wakeup flag
 ** CANIF_WAKEUP_VALIDATION_MASK  Wakeup validation flag
 ** CANIF_TX_CONFIRMATION_MASK    Tx confirmation flag
 ** CANIF_PN_FILTER_ACTIVE_MASK   Partial networking filter flag
 ** CANIF_BUS_MIRRORING_MASK      Mirroring flag
 */
extern VAR( uint8, CANIF_VAR )
  CanIf_CanControllerFlags[];

#endif /* ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
          ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )  || \
          ( CANIF_PUBLIC_PN_SUPPORT == STD_ON) || \
          ( CANIF_BUS_MIRRORING_SUPPORT == STD_ON ) ) */

#if (CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Array of Rx queued data */
extern VAR( uint8, CANIF_VAR ) CanIf_RxDataQueue[];
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/** \brief Temporary buffer for mirroring (payload)
 **
 ** This is used to temporarily store the payload
 ** of the PDUs
 */
extern VAR( uint8, CANIF_VAR )
  CanIf_MirrorBuffPayload[];
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

#define CANIF_STOP_SEC_VAR_CLEARED_8
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_VAR_CLEARED_16
#include <CanIf_MemMap.h>

#if (CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Array of Rx queue Indexes */
extern VAR( uint16, CANIF_VAR ) CanIf_MFRxQueueIdx[];
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT */

#if CANIF_PUBLIC_TX_BUFFERING == STD_ON

/** \brief Array of a HTH's number of pending Tx requests */
extern VAR( CanIf_TxBufferIndexType, CANIF_VAR )
  CanIf_NrOfPendingTxRequests[];

/** \brief Pending transmit request buffers
 **
 ** This array stores pending transmit request IDs. It is named as array (8)
 ** in the CAN interface design.
 */
extern VAR( CanIf_LPduIndexType, CANIF_VAR )
  CanIf_PendingTxBuffers[];

#endif /* CANIF_PUBLIC_TX_BUFFERING */

#if(CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Total size of the queue
 **
 ** This variable shows the current value of the queue of the CAN interface.
 */
extern VAR( uint16, CANIF_VAR ) CanIf_TotalRxQueueSize[];
#endif/* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */

#if(CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
/** \brief Number of Rx Pdus exceeding the queue
 **
 ** This variable shows the current number of Rx Pdu that could not be added in
 ** the queue of the CAN interface.
 */
extern VAR( uint16, CANIF_VAR ) CanIf_NoOfRxPdusExceedingQueue[];
#endif/* CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */

#if((CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON) && (CANIF_TX_CONFIRMATION_SUPPORT == STD_ON))
/** \brief Total size of the queue
 **
 ** This variable shows the current value of the queue of the CAN interface.
 */
extern volatile VAR( uint16, CANIF_VAR ) CanIf_TotalTxQueueSize[];
#endif/* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON && CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */

#if((CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON) && (CANIF_TX_CONFIRMATION_SUPPORT == STD_ON))
/** \brief Number of Tx Pdus exceeding the queue
 **
 ** This variable shows the current number of Tx Pdu that could not be added in
 ** the queue of the CAN interface.
 */
extern volatile VAR( uint16, CANIF_VAR ) CanIf_NoOfTxPdusExceedingQueue[];
#endif/* CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON && CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */


#define CANIF_STOP_SEC_VAR_CLEARED_16
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>

extern VAR( CanIf_ControllerModeType, CANIF_VAR )
  CanIf_CanControllerMode[];

/** \brief PDU modes
 **
 ** This array contains the PDU mode of all configured controllers.
 */
extern VAR( CanIf_PduModeType, CANIF_VAR )
  CanIf_PduMode[];

#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )
/** \brief Array for dynamic CAN IDs
 **
 ** This array contains the current set CAN ID for dynamic Tx L-PDUs.
 */
 extern VAR( Can_IdType, CANIF_VAR )
  CanIf_DynamicTxLPduCanIds[];
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

/** \brief Global Tx L-PDU configuration
 **
 ** This is a pointer to the current global Tx L-PDU configuration.
 */
extern P2CONST( CanIf_TxLPduConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_TxPduConfigPtr;

/** \brief Global CanIf configuration
 **
 ** This is a pointer to the current CanIf configuration.
 */
extern P2CONST( CanIf_ConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_ConfigPtr;

#if CANIF_PUBLIC_TX_BUFFERING == STD_ON

/** \brief Array of Tx buffers
 **
 ** This is the array of Tx buffers used during message transmission if Tx
 ** buffering is enabled.
 */
extern VAR( CanIf_TxBufferType, CANIF_VAR )
  CanIf_TxBuffer[];

#endif /* CANIF_PUBLIC_TX_BUFFERING */

#if (CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON)

/** \brief Array of Tx queue
 **
 ** This is the array of Tx queue used during message transmission
 ** if Tx processing is enabled.
 */
extern VAR( CanIf_TxQueueType, CANIF_VAR )
    CanIf_TxPduQueue[];

#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT */

#if (CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)

/** \brief Array of Rx queue
 **
 ** This is the array of Rx queue used during message reception
 ** if Rx processing is enabled.
 */
extern VAR( CanIf_RxQueueType, CANIF_VAR )
  CanIf_RxPduQueue[];

#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/** \brief Temporary buffer for mirroring
 **
 ** This is used to temporarily store the PDUs
 */
extern VAR( CanIf_MirrorBuffType, CANIF_VAR )
  CanIf_MirrorBuff[];
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* ifndef CANIF_INT_H */
/*==================[end of file]============================================*/
