
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


/*
 *  MISRA-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule 11.8 (Required)
 *    A cast shall not remove any const or volatile qualification from the type pointed
 *    to by a pointer.
 *
 *    Reason:
 *    For performance reasons variable shall be given to the upper layer without copying its
 *    content, which demands an explicit cast.
 */

 /* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: cast-qual
 * warning: cast discards 'const' qualifier from pointer target type [-Wcast-qual]
 *
 * Reason:
 * When RxIndication functions from UL will be modified to accept a pointer to constant,
 * this warning will not show anymore.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason: False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 *
 * TASKING-2) Deviated Rule: W588
 * W588: dead assignment to "CheckedSources" eliminated
 *
 * Reason: The CheckedSources variable is initialized first with CANIF_NO_WAKEUP_SOURCE (value 0).
 *         The CtrlConfigPtr->wakeupSourceOut or TrcvChnlConfigPtr->wakeupSourceOut has value
 *         CANIF_NO_WAKEUP_SOURCE (value 0) and here an OR bitwise operation is done. (ASCCANIF-1203)
 */

/*==================[inclusions]=============================================*/

#ifndef CANIF_NO_PBCFG_REQUIRED
#define CANIF_NO_PBCFG_REQUIRED
#endif
#define CANIF_NO_CFGCLASSMIX_REQUIRED

#include <CanIf_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <TSMem.h>              /* EB memcopy */
#include <ComStack_Types.h>     /* AUTOSAR COM types */
#include <CanIf_Api.h>          /* Module public API and configuration */
#include <CanIf_Cbk.h>          /* Module callback functions */
#include <CanIf_Int.h>          /* CanIf internals */
#include <CanIf_Int_Cfg.h>      /* Generated internal configuration */
#include <CanIf_Lcfg.h>

#include <SchM_CanIf.h>         /* AUTOSAR BSW scheduler */
/* !LINKSTO CanIf.ASR40.CANIF464,2 */
#include <Det.h>                /* AUTOSAR development error tracer */
#include <Can_GeneralTypes.h>   /* AUTOSAR CAN general types */
/* !LINKSTO CanIf.EB.SWS_CANIF_00903,1 */
#if(CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
#include <Mirror.h>
#endif

#if(CANIF_METADATA_SUPPORT == STD_ON)
#include <EcuC.h>
#endif

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef CANIF_VENDOR_ID /* configuration check */
#error CANIF_VENDOR_ID must be defined
#endif

#if (CANIF_VENDOR_ID != 1U) /* vendor check */
#error CANIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef CANIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error CANIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error CANIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef CANIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error CANIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANIF_AR_RELEASE_MINOR_VERSION != 0U )
#error CANIF_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef CANIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error CANIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (CANIF_AR_RELEASE_REVISION_VERSION != 3U )
#error CANIF_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef CANIF_SW_MAJOR_VERSION /* configuration check */
#error CANIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (CANIF_SW_MAJOR_VERSION != 6U)
#error CANIF_SW_MAJOR_VERSION wrong (!= 6U)
#endif

#ifndef CANIF_SW_MINOR_VERSION /* configuration check */
#error CANIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (CANIF_SW_MINOR_VERSION < 10U)
#error CANIF_SW_MINOR_VERSION wrong (< 10U)
#endif

#ifndef CANIF_SW_PATCH_VERSION /* configuration check */
#error CANIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (CANIF_SW_PATCH_VERSION < 15U)
#error CANIF_SW_PATCH_VERSION wrong (< 15U)
#endif



/*==================[macros]=================================================*/

/** \brief Can ID FD type mask
 **
 ** This macro defines the mask value to get the Can ID FD type
 ** (FD/non-FD).
 */
#if (defined CANIF_CANID_FD_TYPE_MASK)
#error CANIF_CANID_FD_TYPE_MASK already defined!
#endif /* #if( defined CANIF_CANID_FD_TYPE_MASK)*/
#define CANIF_CANID_FD_TYPE_MASK 0x40000000U

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
/** \brief Development error trace function
 **
 ** Macro to abstract the development error trace (Det) API function for the
 ** Det checks.
 */
#if (defined CANIF_DET_REPORT_ERROR)
#error CANIF_DET_REPORT_ERROR already defined!
#endif /* #if( defined CANIF_DET_REPORT_ERROR)*/
#define CANIF_DET_REPORT_ERROR (void)Det_ReportError
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

/** \brief Development runtime error trace function
 **
 ** Macro to abstract the development error trace (Det) API function for the
 ** runtime error reporting.
 */
#if (defined CANIF_DET_REPORT_RUNTIME_ERROR)
#error CANIF_DET_REPORT_RUNTIME_ERROR already defined!
#endif /* #if( defined CANIF_DET_REPORT_RUNTIME_ERROR)*/
#define CANIF_DET_REPORT_RUNTIME_ERROR (void)Det_ReportRuntimeError

/** \brief Test condition for message reception mode
 **
 ** This macro defines a condition which can be used in corresponding
 ** statements (e.g. if) to check if the current controller and pdu mode of
 ** the controller with internal index \p ControllerID allows message
 ** reception.
 ** The condition is "true" if the current controller and PDU mode allow
 ** message transmission and delivers "false" if this is not the case.
 **
 ** \param[in] ControllerId CanIf controller index
 */
#if (defined CANIF_CONDITION_RX_NOT_ALLOWED)
#error CANIF_CONDITION_RX_NOT_ALLOWED already defined!
#endif /* #if( defined CANIF_CONDITION_RX_NOT_ALLOWED)*/
#define CANIF_CONDITION_RX_NOT_ALLOWED( ControllerId ) \
  (    ( CanIf_CanControllerMode[(ControllerId)] != CANIF_CS_STARTED ) \
    || ( (CanIf_PduMode[(ControllerId)] != CANIF_ONLINE) \
          && (CanIf_PduMode[(ControllerId)] != CANIF_TX_OFFLINE) \
       )\
  )



/** \brief Macro to read the upper range configuration of the given PDU
 **
 ** \param[in] index Index into the sorted Rx-LPdu config
 **
 ** \return Upper range CAN ID or CANIF_NO_RANGE_CONFIG if no range
 **         configuration is assigned.
 */
#if (defined CANIF_GET_UPPER_RANGE_CANID)
#error CANIF_GET_UPPER_RANGE_CANID already defined!
#endif /* #if( defined CANIF_GET_UPPER_RANGE_CANID)*/
#define CANIF_GET_UPPER_RANGE_CANID( index ) \
  (CANIF_CFG_GET_SORTEDRXLPDU_CFG(index).upperRangeCanId)



/** \brief Macro to check if the PDU can receive the given CAN ID
 **
 ** This macro can be used to decide whether the given PDU is able to receive
 ** a CAN message with the given CAN ID.
 **
 ** \param[in] index Index into the sorted Rx-LPdu config
 ** \param[in] msgCanId CAN ID of the received message
 **
 ** \return Result of the check
 ** \retval E_OK The Rx L-PDU is allowed to receive the given CAN ID
 ** \retval E_NOT_OK The Rx L-PDU may not receive the given CAN ID
 */
#if( CANIF_RANGE_RECEPTION == STD_ON )
#if (defined CANIF_SINGLE_PDU_SW_FILTERING)
#error CANIF_SINGLE_PDU_SW_FILTERING already defined!
#endif /* #if( defined CANIF_SINGLE_PDU_SW_FILTERING)*/
#define CANIF_SINGLE_PDU_SW_FILTERING( index, msgCanId ) \
  ( (Std_ReturnType) \
    ( (    ( (uint32)(msgCanId) == (uint32)CANIF_CFG_GET_SORTEDRXLPDU_CFG(index).canId ) \
        || (    ( CANIF_GET_UPPER_RANGE_CANID( (index) ) != CANIF_NO_RANGE_CONFIG ) \
             && (    ( (uint32)CANIF_CFG_GET_SORTEDRXLPDU_CFG(index).canId <= (uint32)(msgCanId) ) \
                  && ( CANIF_GET_UPPER_RANGE_CANID( (index) ) >= (uint32)(msgCanId) ) \
                ) \
           ) \
      ) \
      ? E_OK \
      : E_NOT_OK \
    ) \
  )
#else
#if (defined CANIF_SINGLE_PDU_SW_FILTERING)
#error CANIF_SINGLE_PDU_SW_FILTERING already defined!
#endif /* #if( defined CANIF_SINGLE_PDU_SW_FILTERING)*/
#define CANIF_SINGLE_PDU_SW_FILTERING( index, msgCanId ) \
  ( (Std_ReturnType) \
    ( ( (uint32)(msgCanId) == (uint32)CANIF_CFG_GET_SORTEDRXLPDU_CFG(index).canId ) \
      ? E_OK \
      : E_NOT_OK \
    ) \
  )
#endif /* CANIF_RANGE_RECEPTION == STD_ON */



/** \brief Converts the CAN id for comparison
 **
 ** This macro converts the given CAN ID for a priority comparison. Standard
 ** IDs are shifted 19 bits to the left while the highest bit of extended
 ** IDs is cleared, then they are shifted 1 bit to the left and finally
 ** incremented by one. In this way a standard ID will have higher priority
 ** than an extended ID with the same base ID (first 11 bit) because the
 ** extended ID compare values least significant bit will be one.
 **
 ** \param[in] id CAN ID (of type Can_IdType)
 **
 ** \retval CAN ID compare value
 */
#if (defined CANIF_CONVERT_ID_FOR_COMPARISON)
#error CANIF_CONVERT_ID_FOR_COMPARISON already defined!
#endif /* #if( defined CANIF_CONVERT_ID_FOR_COMPARISON)*/
#define CANIF_CONVERT_ID_FOR_COMPARISON( id ) \
    ( (((uint32)(id) & 0x80000000UL) == 0UL) ? \
      ((uint32)((0x7FFU&((uint32)(id))) << 19U)) : \
      ((uint32)((((id) & 0x1FFFFFFFUL) << 1U) + 1UL)) )



/** \brief Get CanIf internal HTH index
 **
 ** This macro is used to get the internal index of the Can Driver HTH with
 ** the given ID.
 **
 ** \param[in] CanHth HTH ID in the Can Driver
 **
 ** \return CanIf internal HTH index
 */
#if( CANIF_HOH_TRANSLATION_OPT == STD_ON )

#if (defined CANIF_GET_HTH_INDEX)
#error CANIF_GET_HTH_INDEX already defined!
#endif /* #if( defined CANIF_GET_HTH_INDEX)*/
#define CANIF_GET_HTH_INDEX( CanHth ) ((CanHth) - CanIf_DriverConfigPtr->nrOfHrhs)

#else

#if (defined CANIF_GET_HTH_INDEX)
#error CANIF_GET_HTH_INDEX already defined!
#endif /* #if( defined CANIF_GET_HTH_INDEX)*/
#define CANIF_GET_HTH_INDEX( CanHth ) (CANIF_GET_INT_HOH_IDX(CanHth) & (0xFFFFU^CANIF_HTH_ID_MASK))

#endif



/** \brief Get CanIf internal HRH index
 **
 ** This macro is used to get the internal index of the Can Driver HRH with the
 ** given ID.
 **
 ** \param[in] CanHrh HRH ID in the CAN Driver
 **
 ** \return CanIf internal HRH index
 */
#if( CANIF_HOH_TRANSLATION_OPT == STD_ON )

#if (defined CANIF_GET_HRH_INDEX)
#error CANIF_GET_HRH_INDEX already defined!
#endif /* #if( defined CANIF_GET_HRH_INDEX)*/
#define CANIF_GET_HRH_INDEX( CanHrh ) (CanHrh)

#else

#if (defined CANIF_GET_HRH_INDEX)
#error CANIF_GET_HRH_INDEX already defined!
#endif /* #if( defined CANIF_GET_HRH_INDEX)*/
#define CANIF_GET_HRH_INDEX( CanHrh ) (CANIF_GET_INT_HOH_IDX(CanHrh))
#endif



#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

/** \brief Get index of the Tx buffer for the given Tx PDU
 **
 ** This macro returns the index of the Tx buffer assigned to the given Tx PDU.
 **
 ** Precondition:
 ** - The given Tx PDU must be valid.
 **
 ** \param[in] CanTxPduId Tx PDU, range: 0..Id of last (dynamic) Tx PDU
 **
 ** \return Index of the associated Tx buffer or CANIF_INVALID_TX_BUFFER_INDEX
 **         if no buffer is assigned (i.e. the PDU is assigned to a FULL CAN
 **         HTH).
 */
#if (defined CANIF_GET_TX_BUFFER_INDEX)
#error CANIF_GET_TX_BUFFER_INDEX already defined!
#endif /* #if( defined CANIF_GET_TX_BUFFER_INDEX)*/
#define CANIF_GET_TX_BUFFER_INDEX( CanTxPduId ) \
  (CanIf_TxPduConfigPtr[(CanTxPduId)].bufferIndex)



/** \brief Query if the given Tx buffer is empty
 **
 ** This macro returns the boolean value to indicate if the Tx buffer with the
 ** index \p BufIdx is empty (TRUE) or filled (FALSE).
 ** A Tx buffer is used if the element dlc differs from CANIF_INVALID_DLC.
 **
 ** Precondition:
 ** - The given buffer index must be valid.
 **
 ** \param[in] BufIdx Index of the Tx buffers, range: 0..#Tx buffers - 1
 **
 ** \return Result of the check
 ** \retval TRUE The Tx buffer is empty.
 ** \retval FALSE The Tx buffer is full.
 */
#if (defined CANIF_TX_BUFFER_IS_EMPTY)
#error CANIF_TX_BUFFER_IS_EMPTY already defined!
#endif /* #if( defined CANIF_TX_BUFFER_IS_EMPTY)*/
#define CANIF_TX_BUFFER_IS_EMPTY( BufIdx ) \
  ( ( CanIf_TxBuffer[(BufIdx)].length == CANIF_INVALID_DLC ) ? TRUE : FALSE )



/** \brief Invalidate the given Tx buffer
 **
 ** This macro invalidates the transmit buffer with index \b BufIdx . This
 ** means, that the element swPduHandle in the corresponding Tx buffer is set
 ** to CANIF_INVALID_LPDU_INDEX.
 **
 ** Precondition:
 ** - The given buffer index must be valid.
 **
 ** \param[in] BufIdx Index of the Tx buffers, range: 0..#Tx buffers - 1
 */
#if (defined CANIF_INVALIDATE_TX_BUFFER)
#error CANIF_INVALIDATE_TX_BUFFER already defined!
#endif /* #if( defined CANIF_INVALIDATE_TX_BUFFER)*/
#define CANIF_INVALIDATE_TX_BUFFER( BufIdx ) \
    do \
    { \
      CanIf_TxBuffer[(BufIdx)].length = CANIF_INVALID_DLC; \
    } while(0)



/** \brief Get the CAN ID of a buffered Tx PDU
 **
 ** This function returns the CAN ID of the buffered Tx PDU \p CanTxPduId. It
 ** either directly reads it from the buffer (if at least one dynamic Tx PDU
 ** is configured) or calculates it from the configuration.
 **
 ** Precondition:
 ** - The given Tx PDU ID must be valid.
 ** - There must be a buffered PDU for the given Tx PDU.
 **
 ** \param[in] CanTxPduId Tx PDU, range: 0..Id of last (dynamic) Tx PDU
 **
 ** \return CAN ID for the buffered Tx PDU.
 */
#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )

#if (defined CANIF_GET_BUFFERED_CAN_ID)
#error CANIF_GET_BUFFERED_CAN_ID already defined!
#endif /* #if( defined CANIF_GET_BUFFERED_CAN_ID)*/
#define CANIF_GET_BUFFERED_CAN_ID( CanTxPduId ) \
  (CanIf_TxBuffer[CANIF_GET_TX_BUFFER_INDEX(CanTxPduId)]).id

#else /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

#if (defined CANIF_GET_BUFFERED_CAN_ID)
#error CANIF_GET_BUFFERED_CAN_ID already defined!
#endif /* #if( defined CANIF_GET_BUFFERED_CAN_ID)*/
#define CANIF_GET_BUFFERED_CAN_ID( CanTxPduId ) \
  CANIF_TRANSLATE_TX_CAN_ID( CanIf_TxPduConfigPtr[(CanTxPduId)].canId )

#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

/** \brief Gets value of CAN FD bit
 **
 ** This macro gets the value of the CAN FD bit.
 **
 ** \param[in] canId Can ID for the check
 */
#if (defined CANIF_CANID_GET_FD_BIT)
#error CANIF_CANID_GET_FD_BIT already defined!
#endif /* #if( defined CANIF_CANID_GET_FD_BIT)*/
#define CANIF_CANID_GET_FD_BIT(canId) ( (uint32)(canId) & (uint32)CANIF_CANID_FD_TYPE_MASK )

/** \brief Checks if CAN ID is a FD CAN ID
 **
 ** This macro checks if the bit identifying a CAN ID as Fast Data
 ** CAN ID is set.
 **
 ** \param[in] canId Can ID for the check
 */
#if (defined CANIF_CANID_IS_FD)
#error CANIF_CANID_IS_FD already defined!
#endif /* #if( defined CANIF_CANID_IS_FD)*/
#define CANIF_CANID_IS_FD(canId)  ( CANIF_CANID_GET_FD_BIT(canId) != (uint32)0U )

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

/** \brief Set mode of a CAN controller
 **
 ** This function changes the mode of the given controller to the given
 ** new mode.
 **
 ** Preconditions:
 ** - The parameters must be checked before this function is called because
 **   no additional checks are performed.
 ** - \p ControllerMode may not be CANIF_CS_UNINIT
 ** - The given controller must already be initialized!
 ** - There must be a lock for the current controller or it must be ensured
 **   that this function is not called reentrant in another way.
 **   The caller must take care of that!
 **
 ** \param[in] ControllerId CanIf controller index
 ** \param[in] ControllerMode New mode for the CAN controller
 **
 ** \return Function result
 ** \retval E_OK Function was successful
 ** \retval E_NOT_OK Function failed, mode is consistent
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetControllerModeInternal
  (
    uint8 ControllerId,
    CanIf_ControllerModeType ControllerMode
  );



#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
/** \brief Buffer the given message
 **
 ** This function tries to buffer the given message in the given HTH's queue.
 **
 ** \param[in] CanTxPduId Tx L-PDU ID
 ** \param[in] CanHthIdx Index of the corresponding HTH in the configuration
 ** \param[in] PduInfoPtr CAN PDU info pointer containing the message data
 **
 ** \retval E_OK Message was stored in the HTH message queue
 ** \retval E_NOT_OK Message queuing attempt failed
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_BufferMessage
  (
    const PduIdType CanTxPduId,
    const Can_HwHandleType CanHthIdx,
    CONSTP2VAR( Can_PduType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  );
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */



/** \brief Set PDU mode
 **
 ** This function sets the PDU mode (ONLINE, OFFLINE, etc.) for the controller
 ** with the CanIf internal index \p ControllerId to the request mode given in
 ** \p PduModeRequest.
 **
 ** Preconditions:
 ** - The parameter ControllerId must be checked before this function is called
 **   because no additional check is performed!
 ** - Before this function is called it must be made sure that no mode change
 **   or initialization is in progress!
 **
 ** \param[in] ControllerId CanIf internal controller index
 ** \param[in] PduModeRequest Requested PDU mode
 **
 ** \return Result of function execution
 ** \retval E_OK Function was successful
 ** \retval E_NOT_OK Given mode request was unknown
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SetPduModeInternal
  (
    uint8 ControllerId,
    CanIf_PduModeType PduModeRequest
  );



#if( CANIF_READRXPDU_DATA_API == STD_ON )
/** \brief Write Rx buffer
 **
 ** This function is used to write the data \p CanSduPtr to the internal Rx
 ** buffer.
 **
 ** \param[in] RxLPduConfig Configuration of the Rx L-PDU
 ** \param[in] PduInfoPtr Pointer to the received PDU (data and length)
 **
 ** \remarks This function only needs to be generated if the configuration
 **          says so.
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_WriteRxBuffer
  (
    CONSTP2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST )
      RxLPduConfig,
    CONSTP2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  );
#endif /* ( CANIF_READRXPDU_DATA_API == STD_ON ) */



#if (CANIF_SW_FILTER == STD_ON)
/** \brief CAN interface software filtering
 **
 ** This function performs the software filtering for a received message.
 **
 ** \param[in]  CanIfHrhIdx HRH on which the message was received
 ** \param[in]  CanId CAN identifier of the received message
 ** \param[out] Index Index of the filtered Rx L-PDU
 **
 ** \retval E_OK Rx L-PDU was found
 ** \retval E_NOT_OK No Rx L-PDU was found
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SoftwareFiltering
  (
    const uint16 CanIfHrhIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  );
#endif /* CANIF_SW_FILTER == STD_ON */

#if (CANIF_SW_FILTER == STD_ON)
/** \brief Perform software filtering
 **
 ** This function searches the Rx L-PDU index belonging to the given CAN ID
 ** using different methods depending on the CAN ID configuration.
 **
 ** \param[in]  CanIfHrhIdx HRH on which the message was received
 ** \param[in]  FirstIdx First Rx L-PDU index to be searched
 ** \param[in]  LastIdx Last Rx L-PDU index to be searched
 ** \param[in]  CanId CAN identifier of the received message
 ** \param[out] Index of the filtered Rx L-PDU
 **
 ** \retval E_OK Rx L-PDU was found
 ** \retval E_NOT_OK No Rx L-PDU was found
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_PerformSwFiltering
  (
#if (CANIF_METADATA_SUPPORT == STD_ON)
    const uint16 CanIfHrhIdx,
#endif /* CANIF_METADATA_SUPPORT == STD_ON */
    const CanIf_LPduIndexType FirstIdx,
    CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  );
#endif /* CANIF_SW_FILTER == STD_ON */

#if (CANIF_SW_FILTER == STD_ON)
/** \brief Perform binary software filtering
 **
 ** This function searches the Rx L-PDU index belonging to the given CAN ID.
 **
 ** \param[in]  FirstIdx First Rx L-PDU index to be searched
 ** \param[in]  LastIdx Last Rx L-PDU index to be searched
 ** \param[in]  CanId CAN identifier of the received message
 ** \param[out] Index of the filtered Rx L-PDU
 **
 ** \retval E_OK Rx L-PDU was found
 ** \retval E_NOT_OK No Rx L-PDU was found
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_BinarySwFiltering
  (
    const CanIf_LPduIndexType FirstIdx,
    const CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  );
#endif /* CANIF_SW_FILTER == STD_ON */

#if ((CANIF_METADATA_SUPPORT == STD_ON) && (CANIF_SW_FILTER == STD_ON))
/** \brief Perform linear software filtering
 **
 ** This function searches the Rx L-PDU index belonging to the given CAN ID.
 **
 ** \param[in]  FirstIdx First Rx L-PDU index to be searched
 ** \param[in]  LastIdx Last Rx L-PDU index to be searched
 ** \param[in]  CanId CAN identifier of the received message
 ** \param[out] Index of the filtered Rx L-PDU
 **
 ** \retval E_OK Rx L-PDU was found
 ** \retval E_NOT_OK No Rx L-PDU was found
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_LinearSwFiltering
  (
    const CanIf_LPduIndexType FirstIdx,
    const CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  );
#endif  /* (CANIF_METADATA_SUPPORT == STD_ON) && (CANIF_SW_FILTER == STD_ON) */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
/** \brief Perform Det checks for CanIf_RxIndication arguments
 **
 ** This function performs Det checks for CanIf_RxIndication arguments.
 ** Arguments are the same as for CanIf_RxIndication.
 **
 ** \retval E_OK      No Det error detected.
 ** \retval E_NOT_OK  Det error detected and reported.
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_RxIndication_DetChecks
  (
    Can_HwHandleType Hrh,
    Can_IdType CanId,
    PduLengthType CanDlc,
    P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtr
  );
#endif

#if (CANIF_DLC_CHECK_ENABLED == STD_ON)
/** \brief Perform Dlc check and update Dlc of PduInfoPtr
 **
 ** This function performs Dlc checks for CanIf_RxIndication arguments.
 ** And updates the PduInfoPtr->SduLength with the actual valid value.
 **
 ** \retval E_OK      No Dlc error detected.
 ** \retval E_NOT_OK  Dlc error detected and reported to Det.
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_DlcHandling
  (
    PduLengthType CanDlc,
    P2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST ) RxLPduConfigPtr,
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr
  );
#endif /* (CANIF_DLC_CHECK_ENABLED == STD_ON) */

/** \brief Process the Rx Pdu
 **
 ** This function processes the Rx Pdu received at CanIf_RxIndication
 ** and forwards it to the upper layer.
 **
 ** \param[in] PduInfoPtr   Pointer to the received PDU (data and length)
 ** \param[in] CanId        Received CAN ID
 ** \param[in] RxPduId      Rx L-PDU ID
 ** \param[in] ControllerId CanIf internal controller index
 */
STATIC FUNC(void, CANIF_CODE) CanIf_ProcessRxPdu
(
  P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr,
  Can_IdType CanId,
  CanIf_LPduIndexType RxPduId,
  uint8 ControllerId
);



#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
/** \brief Free controller transmit buffers
 **
 ** This function clears the Tx confirmation flags for the CAN controller
 ** with the index \p ControllerId when enabling Tx confirmation polling support.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must be a valid controller index.
 **
 ** Annotation:
 ** - This whole function is executed as critical section to prevent other
 **   inconsistencies due to usage of other functions.
 **
 ** \param[in] ControllerId CanIf controller index
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_ClearTxConfirmationFlag
  (
    const uint8 ControllerId
  );
#endif /* ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) */


#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

/** \brief Free controller transmit buffers
 **
 ** This function frees (clears) the transmit buffers that are currently
 ** used for pending transmit requests for the CAN controller with the
 ** index \p ControllerId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must be a valid controller index.
 **
 ** Annotation:
 ** - This whole function is executed as critical section to prevent other
 **   inconsistencies due to usage of other functions.
 **
 ** \param[in] ControllerId CanIf controller index
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_FreeControllerTxBuffers
  (
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    const uint8 ControllerId,
    boolean callTxConf
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    const uint8 ControllerId
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
  );



/** \brief Add new Tx request to pending list
 **
 ** This function adds Tx L-PDU \p CanTxPduId to the list of pending Tx
 ** requests of HTH \p Hth meaning that the new request is sorted into this
 ** list.
 **
 ** Precondition:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameters must be checked before this function is called because
 **   this is not done here.
 **
 ** \param[in] CanHthIdx  CanIf internal HTH index
 **                       range: 0..Number of HTHs-1
 ** \param[in] CanTxPduId Index of Tx L-PDU to be added
 **                       range: Tx L-PDUs for the HTH
 **
 ** \remarks This function must be executed in a critical section. The caller
 **          must take care of that!
 ** \remarks Before this function is called it must be checked that the
 **          current controller and PDU mode allow adding a new pending
 **          transmit request!
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_AddPendingTxRequest
  (
    const Can_HwHandleType CanHthIdx,
    const CanIf_LPduIndexType CanTxPduId
  );



/** \brief Send next Tx request
 **
 ** This function searches for the next buffered message to send for hth
 ** \p CanHth. If there are pending transmit requests the one with the
 ** highest priority is taken and a transmission is attempted. If no requests
 ** are pending nothing is done by this function.
 **
 ** Precondition:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter must be checked before this function is called because
 **   this is not done here.
 ** - The caller must make sure that:
 **   * The call is performed in a critical section that cannot be interrupted
 **   * Controller and PDU mode allow a message transmission
 **
 ** \param[in] CanHth CAN HTH, range: CAN driver HTHs
 ** \param[in] ControllerId,   range: CAN IF number of controllers
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SendNextTxRequest
  ( const Can_HwHandleType CanHth, const uint8 ControllerId );



/** \brief Clear Tx requests of given HTH
 **
 ** This function clears all pending Tx requests for the given HTH \p CanHth.
 **
 ** Precondition:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter must be checked before this function is called because
 **   this is not done here.
 ** - The caller must make sure that the call is performed in a critical
 **   section that cannot be interrupted.
 **
 ** \param[in] CanHth CAN HTH, range: CAN driver HTHs
 ** \param[in] ControllerId,   range: CAN IF number of controllers
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_ClearHthTxRequests
  ( const Can_HwHandleType CanHth, const uint8 ControllerId );

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */



/** \brief Setting of internal PDU mode when OFFLINE is requested
 **
 ** This function sets the PDU mode internal for OFFLINE
 **
 ** Preconditions:
 ** - CanIf should be initialized
 ** - The parameter ControllerId must be checked before this function is called
 **   because no additional check is performed!
 **
 ** \param[in] ControllerId CanIf internal controller index
 ** \param[in] OldPduMode OldPDU mode
 **
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SetOffline
  (
    uint8 ControllerId,
    CanIf_PduModeType OldPduMode
  );



#if (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
/** \brief Setting of internal PDU mode when Tx Offline Active mode is requested
 **
 ** This function sets the PDU mode internal for Tx OFFLINE_ACTIVE
 **
 ** Preconditions:
 ** - CanIf should be initialized
 ** - The parameter ControllerId must be checked before this function is called
 **   because no additional check is performed!
 **
 ** \param[in] ControllerId CanIf internal controller index
 ** \param[in] OldPduMode OldPDU mode
 **
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SetTxOfflineActive
  (
    uint8 ControllerId,
    CanIf_PduModeType OldPduMode
  );
#endif /* (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON) */


#if( (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) )
/** \brief Send receive indication
 **
 ** This function calls the receive indication function for the upper layer.
 **
 ** \param[in] userType index of the upper layer associated to this LPdu
 ** \param[in] RxLPduId Target Pdu Id
 ** \param[in] PduInfoPtr Pointer to the received PDU (data and length)
 ** \param[in] CanId received CAN ID
 **
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SendRxIndication
  (
    uint8 userType,
    PduIdType RxLPduId,
    P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr,
    Can_IdType CanId
  );
#endif /* (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) */

#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
/** \brief Send upper layer Tx confirmation
 **
 ** This function sends the Tx confirmation of the given Tx L-PDU to the
 ** upper layer that was specified for this L-PDU.
 **
 ** \param[in] CanTxPduId Tx L-PDU ID
 */
STATIC FUNC( void, CANIF_CODE ) CanIf_SendTxConfirmation
  (
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    PduIdType CanTxPduId,
    Std_ReturnType result
#else /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON  */
    PduIdType CanTxPduId
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
  );

#if( CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON )
/** \brief Queue the received Tx Pdu
 **
 ** This function adds to the queue the information that was specified for this L-PDU.
 **
 ** \param[in] TxPduId      Tx L-PDU ID
 **
 ** \retval E_OK      PDU was added in the queue.
 ** \retval E_NOT_OK  PDU was not added in the queue.
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_InsertTxQueue
  (
    PduIdType TxPduId
  );

/** \brief Init the Tx queue
 **
 ** This function initializes the Tx queue for all specified L-PDUs.
 */
STATIC FUNC(void, CANIF_CODE) CanIf_InitTxQueue
  (
    void
  );
#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON */

#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */

#if(CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Queue the received Rx Pdu
 **
 ** This function adds to the queue the information that was specified for this L-PDU.
 **
 ** \param[in] PduInfoPtr   Pointer to the received PDU (data and length)
 ** \param[in] CanId        Received CAN ID
 ** \param[in] RxPduId      Rx L-PDU ID
 ** \param[in] ControllerId CanIf internal controller index
 **
 ** \retval E_OK      PDU was added in the queue.
 ** \retval E_NOT_OK  PDU was not added in the queue.
 */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_InsertRxQueue
  (
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_CONST ) PduInfoPtr,
    Can_IdType CanId,
    CanIf_LPduIndexType RxPduId,
    uint8 ControllerId
  );

/** \brief Init the Rx queue
 **
 ** This function initializes the Rx queue for all specified L-PDUs.
 */
STATIC FUNC(void, CANIF_CODE) CanIf_InitRxQueue
  (
    void
  );
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/** \brief Resets the Mirror buffer
 **
 ** This function resets the Mirror buffers associated with
 ** the controller
 **
 ** \param[in] ControllerId   ID of the controller
 **
 */
STATIC FUNC(void, CANIF_CODE) CanIf_ResetMirrBuff
(
  uint8 ControllerId
);
#endif

/** \brief Process the length of a Tx Pdu on transmission
 **
 ** This function checks that the length of a Tx Pdu to be transmitted
 ** is correct and truncates it if necessary.
 **
 ** \param[in] CanIfTxPduId        CAN ID of the Tx Pdu
 ** \param[in] RequestedTxLength   Requested length for transmission
 **
 ** \retval  TransmitLength        New processed length.
 */
STATIC FUNC(PduLengthType, CANIF_CODE) CanIf_ProcessTxLength
(
    PduIdType CanIfTxPduId,
    PduLengthType RequestedTxLength
);

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>

  /* --- configuration structures ---------------------------------------- */

/** \brief Global Tx L-PDU configuration */
P2CONST( CanIf_TxLPduConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_TxPduConfigPtr;

/** \brief Global CanIf configuration
 **
 ** This is a pointer to the current CanIf configuration.
 */
P2CONST( CanIf_ConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_ConfigPtr;

#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>



/*==================[internal data]==========================================*/

  /* --- general data ---------------------------------------------------- */

#define CANIF_START_SEC_VAR_INIT_8
#include <CanIf_MemMap.h>

/** \brief Initialization state
 **
 ** This variable shows the current state of the CAN interface.
 ** CANIF_UNINIT            CanIf is not initialized
 ** CANIF_INITIALIZED       CanIf is initialized
 */
STATIC VAR( uint8, CANIF_VAR ) CanIf_State = CANIF_UNINITIALIZED;

#define CANIF_STOP_SEC_VAR_INIT_8
#include <CanIf_MemMap.h>


#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>

/* --- configuration structures ---------------------------------------- */

/** \brief Global Rx L-PDU configuration
 **
 ** This is a pointer to the current global Rx L-PDU configuration.
 */
STATIC P2CONST( CanIf_RxLPduConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_RxPduConfigPtr;

/** \brief Pointer to current driver configurations array
 **
 ** This pointer references the driver configurations array that was given
 ** during CanIf initialization.
 */
STATIC P2CONST( CanIf_CanDriverConfigType, CANIF_VAR, CANIF_APPL_CONST )
  CanIf_DriverConfigPtr;

#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanIf_MemMap.h>



/*==================[external function definitions]==========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ClearTrcvWufFlag_HlpNoPn
  (
    uint8 TransceiverId
  )
{
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* report DET error */
  CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                          CANIF_INSTANCE_ID,
                          CANIF_SID_CLEARTRCVWUFFLAG,
                          CANIF_E_PARAM_TRCV
                        );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  TS_PARAM_UNUSED(TransceiverId);

  return E_NOT_OK;
}

FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckWakeFlag_HlpNoPn
  (
    uint8 TransceiverId
  )
{
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* report DET error */
  CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                          CANIF_INSTANCE_ID,
                          CANIF_SID_CHECKTRCVWAKEFLAG,
                          CANIF_E_PARAM_TRCV
                        );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  TS_PARAM_UNUSED(TransceiverId);

  return E_NOT_OK;
}
#endif /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */

#if CANIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_CODE_CC_BLOCK
#include <CanIf_MemMap.h>
#endif

FUNC( Std_ReturnType, CANIF_CODE) CanIf_IsValidConfig
(
  P2CONST(void,AUTOMATIC,CANIF_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(CanIf_ConfigType,AUTOMATIC,CANIF_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_CANIF_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if (ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform */
    if (TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE )
    {
      /* Validate the post build configuration against the compile time configuration */
      if (CANIF_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the link time configuration */
        if (CanIf_LcfgSignature == ConfigPtr->LcfgSignature)
        {
          /* Validate the post build configuration against the compile time published information
             configuration */
          if (CANIF_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
          {
            /* Indicate that the configuration is valid */
            RetVal = E_OK;
          }
        }
      }
    }
  }

  DBG_CANIF_ISVALIDCONFIG_EXIT(RetVal,voidConfigPtr);
  return RetVal;
}
#if CANIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANIF_STOP_SEC_CODE_CC_BLOCK
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>
#endif

/** \brief CAN interface initialization function */
FUNC( void, CANIF_CODE ) CanIf_Init
  (
    P2CONST( CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST ) ConfigPtr
  )
{
  uint16 i; /* loop variable */

  P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST) LocalConfigPtr = ConfigPtr;

  DBG_CANIF_INIT_ENTRY(ConfigPtr);

  /* Save the configuration pointer */
#if (CANIF_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with an null pointer get the configuration from the
   * post build configuration manager */
  if (LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if (E_OK == PbcfgM_GetConfig( CANIF_MODULE_ID,
                                  CANIF_INSTANCE_ID,
                                  &ModuleConfig)
                                )
    {
      LocalConfigPtr = (P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* CANIF_PBCFGM_SUPPORT_ENABLED == STD_ON */


#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* check whether parameter LocalConfigPtr is a NULL_PTR or invalid*/
  if (LocalConfigPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_INIT,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else if ( E_OK != CanIf_IsValidConfig(LocalConfigPtr))
  {
    /* check that configuration pointer is valid */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_INIT,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT */
  {
    /* perform functionality */
    CanIf_ConfigPtr = LocalConfigPtr;

    /* --- initialize internal variables ----------------------------------- */

    /* --- configuration --- */

    /* global Tx L-PDU configuration */
    CanIf_TxPduConfigPtr = &CANIF_CFG_GET_TXLPDU_CFG(0U);

      /* global Rx L-PDU configuration */
    CanIf_RxPduConfigPtr = &CANIF_CFG_GET_RXLPDU_CFG(0U);


    /* driver configurations */
    CanIf_DriverConfigPtr = &CANIF_CFG_GET_DRIVER_CFG(0U);


          /* --- general variables --- */

#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )
    /* dynamic Tx L-PDU CAN IDs */
    for( i = CanIf_ConfigPtr->nrOfStaticTxLPdus;
         i < CanIf_ConfigPtr->nrOfTxLPdus;
         i++ )
    {
      CanIf_DynamicTxLPduCanIds[i-CanIf_ConfigPtr->nrOfStaticTxLPdus] =
        (CanIf_TxPduConfigPtr[i]).canId;
    }
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

#if( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON )
    /* Tx notifications */
    for( i = 0U; i < (uint16) ((CanIf_ConfigPtr->nrOfTxNotifyPdus+7U)/8U); i++ )
    {
      CanIf_TxLPduNotifyFlags[i] = 0U;
    }
#endif /* CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON */

#if( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
    /* Rx notifications */
    for( i = 0U; i < (uint16) ((CanIf_ConfigPtr->nrOfRxNotifyPdus+7U)/8U); i++ )
    {
      CanIf_RxLPduNotifyFlags[i] = 0U;
    }
#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */

#if( CANIF_READRXPDU_DATA_API == STD_ON )
    /* Rx buffer DLC */
    for( i = 0U; i < CanIf_ConfigPtr->nrOfRxLPdus; i++ )
    {
      CanIf_RxBufferIndexType BufferIndex;  /* buf. index */

      BufferIndex = CanIf_RxPduConfigPtr[i].rxBufferIndex;
      if( BufferIndex != CANIF_INVALID_RX_BUFFER_INDEX )
      {
        /* invalidate Rx buffer DLC for CanIf_ReadRxPduData() to see if a frame
           was received before */
        CanIf_RxBuffer[BufferIndex] = CANIF_INVALID_DLC;
      }
    }
#endif /* CANIF_READRXPDU_DATA_API == STD_ON */

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
    /* number of pending Tx requests */
    for( i = 0U; i < CanIf_DriverConfigPtr->nrOfHths; i++ )
    {
      CanIf_NrOfPendingTxRequests[i] = 0U;
    }
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */



    /* --- Tx buffering --- */

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

    for( i = 0U; i < CanIf_ConfigPtr->nrOfTxBuffers; i++ )
    {
      /* invalidate pending Tx requests */
      CanIf_PendingTxBuffers[i] = CANIF_INVALID_LPDU_INDEX;

      /* mark all Tx buffers as free by invalidating the DLC */
      CanIf_TxBuffer[i].length = CANIF_INVALID_DLC;
    }

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

    /* --- controller and pdu modes --- */

    /* reset controller mode, flags, etc. */
    /* Deviation TASKING-1 */
    for( i = 0U; i < CANIF_GET_NR_OF_CAN_CTRLS(); i++ )
    {

      DBG_CANIF_CANCONTROLLERMODE_GRP(i,(CanIf_CanControllerMode[i]),(CANIF_CS_STOPPED));
      CanIf_CanControllerMode[i] = CANIF_CS_STOPPED;  /* initialize mode variable */
#if( ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
       ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) || \
       ( CANIF_PUBLIC_PN_SUPPORT == STD_ON ) \
     )

      DBG_CANIF_CANCONTROLLERFLAGS_GRP(i,(CanIf_CanControllerFlags[i]),(0U));
      CanIf_CanControllerFlags[i] = CANIF_PN_FILTER_ACTIVE_MASK;         /* SWS_CANIF_00863: enable PnTxFilter during initialization */
#endif /* ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
            ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) || \
            ( CANIF_PUBLIC_PN_SUPPORT == STD_ON ) */

      /* SWS_CANIF_00864: switch every channel to CANIF_OFFLINE */
      DBG_CANIF_PDUMODE_GRP(i,(CanIf_PduMode[i]),(CANIF_OFFLINE));
      CanIf_PduMode[i] = CANIF_OFFLINE;           /* always reset Pdu mode */
    }

#if(CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
    CanIf_InitRxQueue();
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */

#if((CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON) && (CANIF_TX_CONFIRMATION_SUPPORT == STD_ON))
    CanIf_InitTxQueue();
#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON && CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */

    /* CanIf is now initialized */

    DBG_CANIF_STATE((CanIf_State),(CANIF_INITIALIZED));
    CanIf_State = CANIF_INITIALIZED;
  }

  DBG_CANIF_INIT_EXIT(ConfigPtr);
}



/** \brief Controller mode setting function */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetControllerMode
  (
    uint8 ControllerId,
    CanIf_ControllerModeType ControllerMode
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */

  DBG_CANIF_SETCONTROLLERMODE_ENTRY(ControllerId,ControllerMode);
  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETCONTROLLERMODE,
                            CANIF_E_UNINIT
                          );

  }
  /* parameter check */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETCONTROLLERMODE,
                            CANIF_E_PARAM_CONTROLLERID
                          );

  }
  /* controller mode check */
  else if(    (ControllerMode != CANIF_CS_SLEEP)
           && (ControllerMode != CANIF_CS_STARTED)
           && (ControllerMode != CANIF_CS_STOPPED)
         )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETCONTROLLERMODE,
                            CANIF_E_PARAM_CTRLMODE
                          );

  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {

    /* --- perform mode transition ----------------------------------------- */

    /* perform mode switch */
    RetVal = CanIf_SetControllerModeInternal( ControllerId, ControllerMode );
  }

  DBG_CANIF_SETCONTROLLERMODE_EXIT(RetVal,ControllerId,ControllerMode);
  return RetVal;
}



/** \brief Get controller mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetControllerMode
  (
    uint8 ControllerId,
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    /* !LINKSTO CanIf.EB.GetControllerMode.Signature.Mirroring.ON,1 */
    P2VAR( Can_ControllerStateType, AUTOMATIC, CANIF_APPL_DATA )
#else
    /* !LINKSTO CanIf.EB.GetControllerMode.Signature.Mirroring.OFF,1 */
    P2VAR( CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA )
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
      ControllerModePtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */

  DBG_CANIF_GETCONTROLLERMODE_ENTRY(ControllerId,ControllerModePtr);
  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERMODE,
                            CANIF_E_UNINIT
                          );
  }

  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERMODE,
                            CANIF_E_PARAM_CONTROLLERID
                          );

  }

  /* parameter ControllerModePtr */
  else if( ControllerModePtr == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERMODE,
                            CANIF_E_PARAM_POINTER
                          );

  }
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
  {

    /* --- get network mode ------------------------------------------------ */

    /* read controller mode */
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_OFF)
    *ControllerModePtr = CanIf_CanControllerMode[ControllerId];
#else
    switch(CanIf_CanControllerMode[ControllerId])
    {
      /* CHECK: NOPARSE */
      /* The controller is initialized beforehands,
       * the state can't be UNINIT */
      case CANIF_CS_UNINIT:
        *ControllerModePtr = CAN_CS_UNINIT;
        CANIF_UNREACHABLE_CODE_ASSERT(CANIF_SID_GETCONTROLLERMODE);
        break;
      /* CHECK: PARSE */
      case CANIF_CS_SLEEP:
        *ControllerModePtr = CAN_CS_SLEEP;
        break;
      case CANIF_CS_STARTED:
        *ControllerModePtr = CAN_CS_STARTED;
        break;
      case CANIF_CS_STOPPED:
        *ControllerModePtr = CAN_CS_STOPPED;
        break;
      /* CHECK: NOPARSE */
      default:
        /* No other possible value */
        CANIF_UNREACHABLE_CODE_ASSERT(CANIF_SID_GETCONTROLLERMODE);
        break;
      /* CHECK: PARSE */
    }
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

    RetVal = E_OK;
  }

  DBG_CANIF_GETCONTROLLERMODE_EXIT(RetVal,ControllerId,ControllerModePtr);
  return RetVal;
}



/** \brief Transmit an L-PDU */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_Transmit
  (
    PduIdType CanTxPduId,
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  Can_HwHandleType CanHth;          /* CAN HTH */
  Can_PduType PduInfo;              /* CAN PDU */
  uint8 ControllerId;   /*  controller index */
  PduLengthType sduLengthInt;

  boolean send_message = FALSE;
#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
  /* flag for calling the Tx confirmation function */
  boolean CallTxConfirmation = FALSE;
#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */

  DBG_CANIF_TRANSMIT_ENTRY(CanTxPduId,PduInfoPtr);



  /* --- state and parameter checks -------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSMIT,
                            CANIF_E_UNINIT
                          );

  }

  /* parameter CanTxPduId */
  else if( CanTxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSMIT,
                            CANIF_E_INVALID_TXPDUID
                          );

  }

  /* parameter PduInfoPtr */
  else if( PduInfoPtr == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSMIT,
                            CANIF_E_PARAM_POINTER
                          );

  }
  else if( PduInfoPtr->SduDataPtr == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSMIT,
                            CANIF_E_PARAM_POINTER
                          );

  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* Process the requested length for transmission */
    sduLengthInt = CanIf_ProcessTxLength(CanTxPduId, PduInfoPtr->SduLength);
    /* Check if the length is OK */
    if (sduLengthInt != CANIF_INVALID_DLC)
    {
      /* --- initialize internal variables --------------------------------- */
      CanHth = CanIf_TxPduConfigPtr[CanTxPduId].hth;
#if (CANIF_SINGLE_CAN_CTRL_OPT == STD_OFF)
      ControllerId = CanIf_TxPduConfigPtr[CanTxPduId].intCtrlIdx;
#else
      ControllerId = CANIF_SINGLE_CAN_CTRL_ID;
#endif


      /* --- protection against mode changes ------------------------------- */

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* check current controller mode */
      if( CanIf_CanControllerMode[ControllerId] == CANIF_CS_STARTED )
      {
        /* make sure that the current mode allows message transmissions */
        if( CanIf_PduMode[ControllerId] == CANIF_ONLINE )
        {
#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
          /* CANIF750, CANIF752: Reject Tx PDUs if partial networking filter is
             enabled and the PDU is not configured for passing. */
          if(    ( CANIF_CFG_GET_CTRL_CFG(ControllerId).pNFilterEnabled == FALSE )
              || (
                      ( (CanIf_CanControllerFlags[ControllerId] & CANIF_PN_FILTER_ACTIVE_MASK) == 0U )
                   || ( CanIf_TxPduConfigPtr[CanTxPduId].pNFilterFlag == CANIF_PN_FILTER_ALLOW_PDU )
                 )
            )
#endif /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */
          {
            Can_IdType TmpCanId;        /* Temp. CAN ID */

            /* --- prepare message transmission -------------------------- */

#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )
            /* write pdu information */
            if( CanTxPduId >= CanIf_ConfigPtr->nrOfStaticTxLPdus )
            {
              /* dynamic Tx message */
              TmpCanId = CanIf_DynamicTxLPduCanIds[CanTxPduId-CanIf_ConfigPtr->nrOfStaticTxLPdus];
#if(CANIF_METADATA_SUPPORT == STD_ON)
              /* Check if the Pdu has Metadata */
              if(CanIf_TxPduConfigPtr[CanTxPduId].getMetadataPduId != CANIF_INVALID_LPDU_INDEX)
              {
                uint32 Metadata = 0u;

                /* Get the metadata */
                /* !LINKSTO CanIf.EB.Metadata.Functional.1, 1 */
                (void) EcuC_GetMetaDataCanId(CanIf_TxPduConfigPtr[CanTxPduId].getMetadataPduId, PduInfoPtr, &Metadata);

                /* Check if the Pdu has CanId configured */
                if(CanIf_TxPduConfigPtr[CanTxPduId].canId != CANIF_INVALID_CANID)
                {
                  /* CanIfTxPduCanIdMask defines the bits in CanIfTxPduCanId that shall appear in the actual CanId, the other bits are taken from the MetaData. */
                  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00854,1 */
                  TmpCanId = ((TmpCanId & CanIf_TxPduConfigPtr[CanTxPduId].CanIfTxPduCanIdMask) | ((Can_IdType)Metadata & ~CanIf_TxPduConfigPtr[CanTxPduId].CanIfTxPduCanIdMask));
                }
                else
                {
                  /* Whole CanId is taken from Metadata */
                  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00855,1 */
                  TmpCanId = (Can_IdType)Metadata;
                }
              }
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
            }
            else
            {
              /* static Tx message */
              TmpCanId = CanIf_TxPduConfigPtr[CanTxPduId].canId;
            }
#else  /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */
            /* only static Tx messages */
            TmpCanId = CanIf_TxPduConfigPtr[CanTxPduId].canId;
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

            /* now translate the CAN ID */
            PduInfo.id = CANIF_TRANSLATE_TX_CAN_ID(TmpCanId);
#if( (CANIF_TL_TX_CAN_ID_SUPPORT == STD_ON) && (CANIF_DEV_ERROR_DETECT == STD_ON) )
            /* check the translated CAN ID */
            if( CANIF_CHECK_FOR_INVALID_CAN_ID( PduInfo.id ) )
            {
              /* report DET error */
                CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                        CANIF_INSTANCE_ID,
                                        CANIF_SID_TRANSMIT,
                                        CANIF_E_PARAM_CANID
                                      );
            }
            else
#endif /* (CANIF_TL_TX_CAN_ID_SUPPORT == STD_ON) && (CANIF_DEV_ERROR_DETECT == STD_ON) */
            {
              PduInfo.length = (uint8)sduLengthInt;
              PduInfo.sdu = PduInfoPtr->SduDataPtr;
              PduInfo.swPduHandle = CanTxPduId;
              send_message = TRUE;
            }
          }
        }
        else
        {
          /* --- no transmission allowed --------------------------------- */

#if (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
          /* check if transmit simulation is enabled */
          if( CanIf_PduMode[ControllerId] == CANIF_TX_OFFLINE_ACTIVE )
          {
#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
            /* prepare call of Tx confirmation callback */
            CallTxConfirmation = TRUE;
#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */

#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
            /* CANIF740, CANIF734, SWS_CANIF_00072: Store Tx confirmation flag for the
               controller during Tx confirmation simulation. */

            DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])|(CANIF_TX_CONFIRMATION_MASK)));
            CanIf_CanControllerFlags[ControllerId] |= CANIF_TX_CONFIRMATION_MASK;
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

            /* transmission simulation successful */
            RetVal = E_OK;
          }
#endif /* (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON) */

          /* else:
           * return E_NOT_OK, because PDU channel mode is not in the correct state.
           * don't throw a DET error if PDU channel mode is in state CANIF_OFFLINE as
           * requested by CANIF382 (refer to Deviations document) !!
           */
        }
      }
      /* else:
       * return E_NOT_OK, because CCMSM is not in mode CANIF_CS_STARTED.
       * don't throw a DET error as requested by CANIF723 (refer to Deviations document) !!
       */

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* --- send message -------------------------------------------- */
      if(send_message == TRUE)
      {
        Can_ReturnType CanRetVal;   /* CAN return value */

        /* enter critical section:
         * This critical section also includes the call of the external API function
         * Can_Write()!
         * Rationale:
         * The previous solution enters the critical section after the call to Can_Write()
         * and before buffering the message. This leads to race conditions if Can is busy
         * (and the message shall be buffered) and the TxConfirmation of the previous Can
         * message interrupts this function after the return value is set to CAN_BUSY
         * (Can module) but before evaluating this return value and therefore buffering
         * the message.
         * also refer to ASCCANIF-1169
         */
         /* !LINKSTO dsn.CanIf.Preemption2,1 */
        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* Call Can_Write() to transmit the Can message */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
        CanRetVal = CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].Write( CanHth, &PduInfo );
#else
        /* cast to uint8 is done because casting an enum type to another is raising warnings, even if the two types are compatible */
        CanRetVal = (Can_ReturnType)(uint8)CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].Write( CanHth, &PduInfo );
#endif

        if( CanRetVal != CAN_OK )
        {
          /* message transmission failed ... */
#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

          if( CanRetVal == CAN_BUSY )
          {
            /* ... because HTH is busy -> buffer message */
            RetVal = CanIf_BufferMessage( CanTxPduId,
                                          (Can_HwHandleType) CANIF_GET_HTH_INDEX(CanHth + CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].HohIdOffset),
                                          &PduInfo
                                        );
          }
          /* else : ... because CAN transmit function failed -> abort */
#else /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */
          /* ... so return E_NOT_OK (default return value) */
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */
        }
        else
        {
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
          if(CANIF_BUS_MIRRORING_MASK == (CanIf_CanControllerFlags[ControllerId] & CANIF_BUS_MIRRORING_MASK))
          {
            TS_MemCpy(&CanIf_MirrorBuffPayload[CanIf_TxPduConfigPtr[CanTxPduId].mirrBuffOfs], PduInfo.sdu, PduInfo.length);
            CanIf_MirrorBuff[CanTxPduId].status = CANIF_FRAME_READY_TO_MIRROR;
            CanIf_MirrorBuff[CanTxPduId].length = PduInfo.length;
          }
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
          /* message transmission successful */
          RetVal = E_OK;
        }

        /* leave critical section */
        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
      }

      /* Checking whether Transmission can be notified to UL */
      if(RetVal == E_OK)
      {
#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
        /* call Tx confirmation callback if necessary */
        if( CallTxConfirmation == TRUE )
        {
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
          /* !LINKSTO CanIf.EB.User_TxConfirmation.Signature.Result.ON, 1 */
          CanIf_SendTxConfirmation( CanTxPduId, E_OK);
#else /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON  */
          /* !LINKSTO CanIf.EB.User_TxConfirmation.Signature.Result.OFF, 1 */
          CanIf_SendTxConfirmation( CanTxPduId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
        }
#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */
      }
    }
  }

  DBG_CANIF_TRANSMIT_EXIT(RetVal,CanTxPduId,PduInfoPtr);
  return RetVal;
}



#if( CANIF_SET_BAUDRATE_API == STD_ON )
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetBaudrate
  (
    uint8 ControllerId,
    uint16 BaudRateConfigID
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */

  DBG_CANIF_SETBAUDRATE_ENTRY(ControllerId,BaudRateConfigID);
  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETBAUDRATE,
                            CANIF_E_UNINIT
                          );
  }

  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETBAUDRATE,
                            CANIF_E_PARAM_CONTROLLERID
                          );

  }
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
  {
      /* Call Can_SetBaudrate() to wakeup the can controller */
      RetVal = CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].SetBaudrate(CANIF_GET_CANDRV_CTRL_ID(ControllerId), BaudRateConfigID);
  }

  DBG_CANIF_SETBAUDRATE_EXIT(RetVal,ControllerId,BaudRateConfigID);
  return RetVal;
}
#endif /* CANIF_SET_BAUDRATE_API == STD_ON */


#if( CANIF_CANCEL_TRANSMIT_SUPPORT == STD_ON )
/** \brief Cancel transmit dummy function */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CancelTransmit
  (
    PduIdType CanTxPduId
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */

  DBG_CANIF_CANCELTRANSMIT_ENTRY(CanTxPduId);
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CANCELTRANSMIT,
                            CANIF_E_UNINIT
                          );

  }

  /* parameter CanTxPduId */
  else if( CanTxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CANCELTRANSMIT,
                            CANIF_E_INVALID_TXPDUID
                          );
  }
  else
#else
  /* dummy function */
  TS_PARAM_UNUSED( CanTxPduId );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    RetVal = E_OK;
  }


  DBG_CANIF_CANCELTRANSMIT_EXIT(RetVal,CanTxPduId);
  return RetVal;
}
#endif /* CANIF_CANCEL_TRANSMIT_SUPPORT == STD_ON */



#if( CANIF_READRXPDU_DATA_API == STD_ON )
/** \brief Read received data */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ReadRxPduData
  (
    PduIdType CanRxPduId,
    P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK;   /* return value */
  uint8 i;                            /* loop variable */
  uint8 CanDlc;                       /* received DLC */

  DBG_CANIF_READRXPDUDATA_ENTRY(CanRxPduId,PduInfoPtr);



  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READRXPDUDATA,
                            CANIF_E_UNINIT
                          );

  }

  /* parameter PduInfoPtr */
  else if( ( PduInfoPtr == NULL_PTR ) ||
      ( PduInfoPtr->SduDataPtr == NULL_PTR ) )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READRXPDUDATA,
                            CANIF_E_PARAM_POINTER
                          );
  }

    /* parameter CanRxPduId */
  else if( (CanRxPduId >= CanIf_ConfigPtr->nrOfRxLPdus) || (CanIf_RxPduConfigPtr[CanRxPduId].rxBufferIndex == CANIF_INVALID_RX_BUFFER_INDEX) )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READRXPDUDATA,
                            CANIF_E_INVALID_RXPDUID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {

    /* --- initialize internal variables ----------------------------------- */

        /* buffer index */
        CanIf_RxBufferIndexType BufferIdx = CanIf_RxPduConfigPtr[CanRxPduId].rxBufferIndex;

    /* --- read data from Rx buffer ---------------------------------------- */

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* Check that a valid message was received and copy the value to the return
         parameter.
         The DLC stored in the first byte of the buffer is initialized to the
         value CANIF_INVALID_DLC. If the DLC differs from this value,
         a message was received in the meantime. */
      CanDlc = CanIf_RxBuffer[BufferIdx];
      if( CanDlc != CANIF_INVALID_DLC )
      {
        /* set BufferIdx to start of data by incrementing it by one (first byte
           is DLC) */
        BufferIdx++;

        /* fill given pointer */
        PduInfoPtr->SduLength = CanDlc;
        for( i = 0U; i < CanDlc; i++ )
        {
          (PduInfoPtr->SduDataPtr)[i] = CanIf_RxBuffer[BufferIdx+i];
        }

        /* function was successful */
        RetVal = E_OK;
      }

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
    }


  DBG_CANIF_READRXPDUDATA_EXIT(RetVal,CanRxPduId,PduInfoPtr);
  return RetVal;
}
#endif /* CANIF_READRXPDU_DATA_API == STD_ON */



#if( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Read Tx notification status */
FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_ReadTxNotifStatus
  ( PduIdType CanTxPduId )
{
  CanIf_NotifStatusType RetVal = CANIF_NO_NOTIFICATION;

  DBG_CANIF_READTXNOTIFSTATUS_ENTRY(CanTxPduId);
                                                    /* return value */

  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READTXNOTIFSTATUS,
                            CANIF_E_UNINIT
                          );
  }

  /* parameter CanTxPduId */
  else if( CanTxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READTXNOTIFSTATUS,
                            CANIF_E_INVALID_TXPDUID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {


    /* --- initialize internal variables ----------------------------------- */

    /* notification idx. */
    const CanIf_LPduIndexType NotifIdx = CanIf_TxPduConfigPtr[CanTxPduId].notifyIndex;

    /* --- continue parameter checks --------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* configuration of L-PDU CanTxPduId */
    if( NotifIdx == CANIF_INVALID_LPDU_INDEX )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_READTXNOTIFSTATUS,
                              CANIF_E_INVALID_TXPDUID
                            );
    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {


      /* --- read and reset notification ------------------------------------- */

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* read the transmit notification flag */
      if( CANIF_GET_NOTIFICATION_FLAG( CanIf_TxLPduNotifyFlags, NotifIdx ) != 0U )
      {
        RetVal = CANIF_TX_RX_NOTIFICATION;
      }

      /* clear transmit notification flag */
      CANIF_CLEAR_NOTIFICATION_FLAG( CanIf_TxLPduNotifyFlags, NotifIdx );

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
    }
  }

  DBG_CANIF_READTXNOTIFSTATUS_EXIT(RetVal,CanTxPduId);
  return RetVal;
}
#endif /* CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON */



#if( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Read Rx notification status */
FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_ReadRxNotifStatus
  ( PduIdType CanRxPduId )
{
  CanIf_NotifStatusType RetVal = CANIF_NO_NOTIFICATION;
                                                    /* return value */
  CanIf_LPduIndexType NotifIdx;   /* notification idx. */

  DBG_CANIF_READRXNOTIFSTATUS_ENTRY(CanRxPduId);



  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READRXNOTIFSTATUS,
                            CANIF_E_UNINIT
                          );
  }

  /* parameter CanRxPduId */
  else if( CanRxPduId >= CanIf_ConfigPtr->nrOfRxLPdus )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_READRXNOTIFSTATUS,
                            CANIF_E_INVALID_RXPDUID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {


    /* --- initialize internal variables ----------------------------------- */

    NotifIdx = CanIf_RxPduConfigPtr[CanRxPduId].notifyIndex;



    /* --- continue parameter checks --------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* configuration of L-PDU CanRxPduId */
    if( NotifIdx == CANIF_INVALID_LPDU_INDEX )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_READRXNOTIFSTATUS,
                              CANIF_E_INVALID_RXPDUID
                            );

    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {


      /* --- read and reset notification ------------------------------------- */

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* read the receive notification flag */
      if( CANIF_GET_NOTIFICATION_FLAG( CanIf_RxLPduNotifyFlags, NotifIdx ) != 0U )
      {
        RetVal = CANIF_TX_RX_NOTIFICATION;
      }

      /* clear receive notification flag */
      CANIF_CLEAR_NOTIFICATION_FLAG( CanIf_RxLPduNotifyFlags, NotifIdx );

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
    }
  }

  DBG_CANIF_READRXNOTIFSTATUS_EXIT(RetVal,CanRxPduId);
  return RetVal;
}
#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */



/** \brief Set requested L-PDU channel mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetPduMode
  (
    uint8 ControllerId,
    CanIf_PduModeType PduModeRequest
  )
{
  Std_ReturnType RetVal = E_NOT_OK;   /* return value */

  DBG_CANIF_SETPDUMODE_ENTRY(ControllerId,PduModeRequest);
  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETPDUMODE,
                            CANIF_E_UNINIT
                          );
  }
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETPDUMODE,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }
#if (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
  else if(PduModeRequest > CANIF_ONLINE)
#else /* (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_OFF) */
  else if((PduModeRequest > CANIF_ONLINE) || (PduModeRequest == CANIF_TX_OFFLINE_ACTIVE))
#endif /* (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON) */
  {
     /* report DET error */
     CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_SETPDUMODE,
                              CANIF_E_PARAM_PDU_MODE
                           );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* SWS_CANIF_00874: check if controller mode is in CANIF_CS_STARTED */
    if (CanIf_CanControllerMode[ControllerId] == CANIF_CS_STARTED)
    {
      /* --- perform mode transition ----------------------------------------- */

      /* call internal function */
      CanIf_SetPduModeInternal( ControllerId, PduModeRequest );

      RetVal =  E_OK;
    }
  }

  DBG_CANIF_SETPDUMODE_EXIT(RetVal,ControllerId,PduModeRequest);
  return RetVal;
}



/** \brief Read a L-PDU channel mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetPduMode
  (
    uint8 ControllerId,
    P2VAR( CanIf_PduModeType, AUTOMATIC, CANIF_APPL_DATA ) PduModePtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK;   /* return value */

  DBG_CANIF_GETPDUMODE_ENTRY(ControllerId,PduModePtr);
  /* --- perform parameter checks ---------------------------------------- */

#if(CANIF_DEV_ERROR_DETECT == STD_ON)
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETPDUMODE,
                            CANIF_E_UNINIT
                          );
  }

  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETPDUMODE,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }

  /* parameter PduModePtr */
  else if( PduModePtr == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETPDUMODE,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
  {
    /* --- read network channel mode --------------------------------------- */

    /* read the PDU mode */
    *PduModePtr = CanIf_PduMode[ControllerId];
    RetVal = E_OK;
  }

  DBG_CANIF_GETPDUMODE_EXIT(RetVal,ControllerId,PduModePtr);
  return RetVal;
}


#if( CANIF_VERSION_INFO_API == STD_ON )
/** \brief Return module version information */
FUNC( void, CANIF_CODE ) CanIf_GetVersionInfo
  ( P2VAR( Std_VersionInfoType, AUTOMATIC, CANIF_APPL_DATA ) Versioninfo )
{
#if( CANIF_DEV_ERROR_DETECT == STD_ON )

  DBG_CANIF_GETVERSIONINFO_ENTRY(Versioninfo);
  if( Versioninfo == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETVERSIONINFO,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* get version info of this module */
    Versioninfo->vendorID         = CANIF_VENDOR_ID;
    Versioninfo->moduleID         = CANIF_MODULE_ID;
    Versioninfo->sw_major_version = CANIF_SW_MAJOR_VERSION;
    Versioninfo->sw_minor_version = CANIF_SW_MINOR_VERSION;
    Versioninfo->sw_patch_version = CANIF_SW_PATCH_VERSION;
  }

  DBG_CANIF_GETVERSIONINFO_EXIT(Versioninfo);
}
#endif /* CANIF_VERSION_INFO_API == STD_ON */



#if( CANIF_SETDYNAMICTXID_API == STD_ON )
/** \brief Set CAN ID of dynamic Tx L-PDU */
FUNC( void, CANIF_CODE ) CanIf_SetDynamicTxId
  (
    PduIdType CanTxPduId,
    Can_IdType CanId
  )
{
  DBG_CANIF_SETDYNAMICTXID_ENTRY(CanTxPduId,CanId);

  /* --- perform parameter checks ---------------------------------------- */
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */

  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETDYNAMICTXID,
                            CANIF_E_UNINIT
                          );
  }

  /* parameter CanTxPduId */
  else if(    ( CanTxPduId < CanIf_ConfigPtr->nrOfStaticTxLPdus )
      || ( CanTxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
    )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETDYNAMICTXID,
                            CANIF_E_INVALID_TXPDUID
                          );
  }

  /* parameter CanId */
  else if( CANIF_CHECK_FOR_INVALID_CAN_ID( CanId ) )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETDYNAMICTXID,
                            CANIF_E_PARAM_CANID
                          );
  }

  /* Can ID type changes no allowed */
  else if(    CANIF_CANID_GET_FD_BIT(CanId)
           != CANIF_CANID_GET_FD_BIT
                (
                  CanIf_DynamicTxLPduCanIds[CanTxPduId - CanIf_ConfigPtr->nrOfStaticTxLPdus]
                )
         )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETDYNAMICTXID,
                            CANIF_E_PARAM_CANID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {


    /* --- set dynamic CAN id ---------------------------------------------- */

    /* set Tx L-PDU ID to new value */
    CanIf_AtomicAssignCanId(
      CanIf_DynamicTxLPduCanIds[CanTxPduId - CanIf_ConfigPtr->nrOfStaticTxLPdus], CanId );

  }

  DBG_CANIF_SETDYNAMICTXID_EXIT(CanTxPduId,CanId);
}
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON */



#if( CANIF_TRCV_SUPPORT == STD_ON )
/** \brief Set transceiver mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetTrcvMode
  (
    uint8 TransceiverId,
    CanTrcv_TrcvModeType TransceiverMode
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  uint8 TrcvDrvIdx;/* trcv. drv. index */

  DBG_CANIF_SETTRCVMODE_ENTRY(TransceiverId,TransceiverMode);

  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRANSCEIVERMODE,
                            CANIF_E_UNINIT
                          );
  }
  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRANSCEIVERMODE,
                            CANIF_E_PARAM_TRCV
                          );
  }
  /* transceiver mode */
  else if(    (TransceiverMode != CANTRCV_TRCVMODE_STANDBY)
           && (TransceiverMode != CANTRCV_TRCVMODE_SLEEP)
           && (TransceiverMode != CANTRCV_TRCVMODE_NORMAL)
         )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRANSCEIVERMODE,
                            CANIF_E_PARAM_TRCVMODE
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].setOpMode
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId,
                 TransceiverMode
               );
  }

  DBG_CANIF_SETTRCVMODE_EXIT(RetVal,TransceiverId,TransceiverMode);
  return RetVal;
}
#endif /* CANIF_TRCV_SUPPORT == STD_ON */



#if(  CANIF_TRCV_SUPPORT == STD_ON )
/** \brief Get transceiver mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetTrcvMode
  (
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    uint8 TransceiverId,
    P2VAR( CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA )
      TransceiverModePtr
#else
    P2VAR( CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA )
      TransceiverModePtr,
    uint8 TransceiverId
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
  )
{
  uint8 TrcvDrvIdx;                 /* trcv. drv. index */
  Std_ReturnType RetVal = E_NOT_OK; /* return value */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
  DBG_CANIF_GETTRCVMODE_ENTRY(TransceiverId,TransceiverModePtr);
#else
  DBG_CANIF_GETTRCVMODE_ENTRY(TransceiverModePtr,TransceiverId);
#endif


  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRANSCEIVERMODE,
                            CANIF_E_UNINIT
                          );
  }

  /* --- perform parameter checks ---------------------------------------- */

  /* mode pointer */
  else if( TransceiverModePtr == NULL_PTR )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRANSCEIVERMODE,
                            CANIF_E_PARAM_POINTER
                          );
  }
  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRANSCEIVERMODE,
                            CANIF_E_PARAM_TRCV
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].getOpMode
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId,
                 TransceiverModePtr
               );
  }

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
  DBG_CANIF_GETTRCVMODE_EXIT(RetVal,TransceiverId,TransceiverModePtr);
#else
  DBG_CANIF_GETTRCVMODE_EXIT(RetVal,TransceiverModePtr,TransceiverId);
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
  return RetVal;
}
#endif /*  CANIF_TRCV_SUPPORT == STD_ON */



#if (CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON)
/** \brief Get transceiver wakeup reason */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetTrcvWakeupReason
  (
    uint8 TransceiverId,
    P2VAR( CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA )
      TrcvWuReasonPtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  uint8 TrcvDrvIdx;                 /* trcv. drv. index */

  DBG_CANIF_GETTRCVWAKEUPREASON_ENTRY(TransceiverId,TrcvWuReasonPtr);

  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRCVWAKEUPREASON,
                            CANIF_E_UNINIT
                          );
  }

  /* --- perform parameter checks ---------------------------------------- */

  /* wakeup reason pointer */
  else if( TrcvWuReasonPtr == NULL_PTR )
  {

  /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRCVWAKEUPREASON,
                            CANIF_E_PARAM_POINTER
                          );
  }
  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTRCVWAKEUPREASON,
                            CANIF_E_PARAM_TRCV
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].getBusWuReason
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId,
                 TrcvWuReasonPtr
               );
  }

  DBG_CANIF_GETTRCVWAKEUPREASON_EXIT(RetVal,TransceiverId,TrcvWuReasonPtr);
  return RetVal;
}
#endif /*  CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON */



#if (CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON)
/** \brief Set transceiver wakeup mode */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetTrcvWakeupMode
  (
    uint8 TransceiverId,
    CanTrcv_TrcvWakeupModeType TrcvWakeupMode
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  uint8 TrcvDrvIdx;                 /* trcv. drv. index */

  DBG_CANIF_SETTRCVWAKEUPMODE_ENTRY(TransceiverId,TrcvWakeupMode);

  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRCVWAKEUPMODE,
                            CANIF_E_UNINIT
                          );
  }

  /* --- perform parameter checks ---------------------------------------- */

  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRCVWAKEUPMODE,
                            CANIF_E_PARAM_TRCV
                          );
  }
  /* transceiver wakeup mode */
  else if(    (TrcvWakeupMode != CANTRCV_WUMODE_ENABLE)
           && (TrcvWakeupMode != CANTRCV_WUMODE_DISABLE)
           && (TrcvWakeupMode != CANTRCV_WUMODE_CLEAR)
         )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_SETTRCVWAKEUPMODE,
                            CANIF_E_PARAM_TRCVWAKEUPMODE
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].setWakeupMode
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId,
                 TrcvWakeupMode
               );
  }

  DBG_CANIF_SETTRCVWAKEUPMODE_EXIT(RetVal,TransceiverId,TrcvWakeupMode);
  return RetVal;
}
#endif /* CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON */



#if( CANIF_WAKEUP_SUPPORT_API == STD_ON )
/** \brief Check for Can and CanTrcv wakeups */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckWakeup
  (
    EcuM_WakeupSourceType WakeupSource
  )
{
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  boolean WakeupSourceValid = FALSE;  /* valid. flag */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  Std_ReturnType Result = E_NOT_OK;   /* function result */
  EcuM_WakeupSourceType CheckedSources =
    CANIF_NO_WAKEUP_SOURCE;           /* bit field of checked wakeup sources */
  uint8_least i;                      /* loop variable */
#if( CANIF_CAN_WAKEUP_SUPPORT == STD_ON )
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
  boolean CtrlNotSleep = FALSE;       /* Det error CANIF_E_NOT_SLEEP */
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */
#endif /* CANIF_CAN_WAKEUP_SUPPORT == STD_ON */

  DBG_CANIF_CHECKWAKEUP_ENTRY(WakeupSource);

  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKWAKEUP,
                            CANIF_E_UNINIT
                          );
  }
  else
  /* Wakeup source parameter is validated implicitly by the following loops.
     If the mask contains a valid CAN wakeup source one of the loop runs will
     find it and WakeupSourceValid will be set to TRUE. Otherwise no Can and
     CanTrcv API was called and the Det will be triggered after the loops. */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {

#if( CANIF_CAN_WAKEUP_SUPPORT == STD_ON )
    /* --- collect CAN controller data ------------------------------------- */

    boolean CheckWakeUp;

    /* for each controller ... */
    /* Deviation TASKING-1 */
    for( i = 0U; i < CANIF_GET_NR_OF_CAN_CTRLS(); i++ )
    {
      P2CONST( CanIf_CanControllerConfigType, AUTOMATIC, CANIF_APPL_CONST ) CtrlConfigPtr;

      CheckWakeUp = FALSE;
      CtrlConfigPtr = &(CANIF_CFG_GET_CTRL_CFG(i)); /* controller configuration */

#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* ... check that the controller is capable of a req. wakeup ... */
      if( (CtrlConfigPtr->wakeupSourceIn & WakeupSource) != 0U )
      {
        /* ..., that the controller is in mode SLEEP ... */
        if( CanIf_CanControllerMode[i] == CANIF_CS_SLEEP )
        {
          CheckWakeUp = TRUE;
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
          /* update of wakeup mask validation flag */
          WakeupSourceValid = TRUE;
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
        }
        else
        {
          /* ... and abort if not (CANIF679) */
          CtrlNotSleep = TRUE;
        }
      }

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      if(CtrlNotSleep == TRUE)
      {
        break;
      }
#else
      /* ... check that the controller is capable of a req. wakeup ... */
      if( (CtrlConfigPtr->wakeupSourceIn & WakeupSource) != 0U )
      {
        CheckWakeUp = TRUE;
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
        /* update of wakeup mask validation flag */
        WakeupSourceValid = TRUE;
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      }
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */

      /* ... and call Can_CheckWakeup() in this case */
      if(CheckWakeUp == TRUE)
      {
        if( CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(i)].CheckWakeup( CANIF_GET_CANDRV_CTRL_ID(i) ) == CAN_OK )
        {
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
          /* enter critical section */
          SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

          /* a wakeup was detected */
          /* Deviation TASKING-2 */
          CheckedSources |= CtrlConfigPtr->wakeupSourceOut;

          /* set new controller mode according to Can SWS */

          DBG_CANIF_CANCONTROLLERMODE_GRP(i,(CanIf_CanControllerMode[i]),(CANIF_CS_STOPPED));
          CanIf_CanControllerMode[i] = CANIF_CS_STOPPED;

          /* leave critical section */
          SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */

          Result = E_OK;
        }
      }
    }

#if(CANIF_DEV_ERROR_DETECT == STD_ON )
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
    /* !LINKSTO CanIf.EB.CANIF679_1, 1 */
    /* !LINKSTO CanIf.EB.CANIF679_2, 1 */
    if( CtrlNotSleep == TRUE )
    {
      /* ... development error ... */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_CHECKWAKEUP,
                              CANIF_E_NOT_SLEEP
                            );

      /* ... and return E_NOT_OK in this case */
      Result = E_NOT_OK;
    }
    else
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
#endif /* CANIF_CAN_WAKEUP_SUPPORT == STD_ON */
    {
      /* --- collect CAN transceiver data ---------------------------------- */

#if( CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON )
      /* for all transceiver channels ... */
      for( i = 0U; i < CanIf_ConfigPtr->nrOfTransceivers; i++ )
      {
        P2CONST( CanIf_CanTrcvChnlConfigType, AUTOMATIC, CANIF_APPL_CONST )
          TrcvChnlConfigPtr = &(CANIF_CFG_GET_TRCV_CFG(i));
                                                  /* transceiver configuration */

        /* ... check if they support one of the wakeup events ... */
        if( (TrcvChnlConfigPtr->wakeupSourceIn & WakeupSource) != 0U )
        {
          /* ... and call CanTrcv_CB_WakeupByBus() in this case */
          if( CanIf_CanTrcvConfig[TrcvChnlConfigPtr->trcvDrvIdx].
                chkWakeupByBus( TrcvChnlConfigPtr->trcvChnlId ) == E_OK
            )
          {
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
            /* Deviation TASKING-2 */
            CheckedSources |= TrcvChnlConfigPtr->wakeupSourceOut;
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */
            Result = E_OK;
          }

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
          /* update of valid wakeup mask flag */
          WakeupSourceValid = TRUE;
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
        }
      }
#endif /* CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON */



      /* --- wakeup source parameter check --------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
      /* wakeup source validation */
      if( WakeupSourceValid == FALSE )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_CHECKWAKEUP,
                                CANIF_E_PARAM_WAKEUPSOURCE
                              );

      }
      else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */



      /* --- set wakeup events --------------------------------------------- */
      {
#if( CANIF_WAKEUP_VALIDATION_API == STD_ON )
#if( CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF )
        /* CANIF722, CANIF458: set wakeup flag for controllers validating
           the verified wakeup sources */

        /* enter critical section */
        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* Deviation TASKING-1 */
        for( i = 0U; i < CANIF_GET_NR_OF_CAN_CTRLS(); i++ )
        {
          if( (CANIF_CFG_GET_CTRL_CFG(i).wakeupSourceOut & CheckedSources) != 0U )
          {

            DBG_CANIF_CANCONTROLLERFLAGS_GRP(i,(CanIf_CanControllerFlags[i]),((CanIf_CanControllerFlags[i])|(CANIF_WAKEUP_MASK)));
            CanIf_CanControllerFlags[i] |= CANIF_WAKEUP_MASK;
          }
        }

        /* leave critical section */
        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
#endif /* CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF */
#endif /* CANIF_WAKEUP_VALIDATION_API == STD_ON */


        /* --- call upper layer notification --------------------------------- */
        /* !LINKSTO CanIf.EB.Rev2.Wakeup, 2 */
        CANIF_SET_WAKEUP_EVENT( CheckedSources );
      }
    }
  }

  DBG_CANIF_CHECKWAKEUP_EXIT(Result,WakeupSource);
  return Result;
}



#if( CANIF_WAKEUP_VALIDATION_API == STD_ON )
/** \brief Check for validated wakeup events */
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckValidation
  (
    EcuM_WakeupSourceType WakeupSource
  )
{
  Std_ReturnType Result = E_NOT_OK;   /* function result */
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  boolean WakeupSourceValid = FALSE;  /* valid. flag */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  EcuM_WakeupSourceType CheckedSources =
    CANIF_NO_WAKEUP_SOURCE;          /* bit field of checked wakeup sources */
  uint8_least i;                     /* loop variable */

  DBG_CANIF_CHECKVALIDATION_ENTRY(WakeupSource);



  /* --- perform parameter checks ---------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKVALIDATION,
                            CANIF_E_UNINIT
                          );
  }
  else
  /* Wakeup source parameter is validated implicitly by the following loops.
     If the mask contains a valid CAN wakeup source one of the loop runs will
     find it and WakeupSourceValid will be set to TRUE. Otherwise no Can and
     CanTrcv API was called and the Det will be triggered after the loops. */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {


    /* --- check controller wakeup validation flags ------------------------ */

    /* enter critical section */
    SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

    /* Deviation TASKING-1 */
    for( i = 0U; i < CANIF_GET_NR_OF_CAN_CTRLS(); i++ )
    {
      P2CONST( CanIf_CanControllerConfigType, AUTOMATIC, CANIF_APPL_CONST )
        CtrlConfigPtr = &(CANIF_CFG_GET_CTRL_CFG(i));
                                                /* controller configuration */

      /* check that the controller handles a correct wakeup */
      if( (CtrlConfigPtr->wakeupSourceOut & WakeupSource) != 0U )
      {
        /* check if a wakeup event was validated by this controller */
        if( (CanIf_CanControllerFlags[i] & CANIF_WAKEUP_VALIDATION_MASK) != 0U )
        {
          uint8 value;
          CheckedSources |= CtrlConfigPtr->wakeupSourceOut;

#if( CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF )
          /* Clear wakeup and validation flag */
          value = (CanIf_CanControllerFlags[i])&
                        ((uint8)(~(CANIF_WAKEUP_MASK | CANIF_WAKEUP_VALIDATION_MASK)));
#else
          /* Clear validation flag */
          value = (CanIf_CanControllerFlags[i])& ((uint8)(~CANIF_WAKEUP_VALIDATION_MASK));
#endif /* CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF */

          DBG_CANIF_CANCONTROLLERFLAGS_GRP(i,(CanIf_CanControllerFlags[i]), value);
          CanIf_CanControllerFlags[i] = value;
        }

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
        /* update of wakeup mask validation flag */
        WakeupSourceValid = TRUE;
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      }
    }

    /* leave critical section */
    SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();



    /* --- wakeup source parameter check ----------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* wakeup source validation */
    if( WakeupSourceValid == FALSE )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_CHECKVALIDATION,
                              CANIF_E_PARAM_WAKEUPSOURCE
                            );
    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

    {

      /* --- call wakeup validation notification function -------------------- */

      /* RfC 57883:
         If a wakeup event is not validated, CanIf must not indicate EcuM.
         Rationale: The call stops the EcuMValidationTimeout.
         Therefore the EcuM does not detect that no valid wakeup was detected.
         This supersedes requirement CANIF681. */
      if( CheckedSources != 0U )
      {
        CANIF_VALIDATE_WAKEUP_EVENT( CheckedSources );
      }

      Result = E_OK;
    }
  }

  DBG_CANIF_CHECKVALIDATION_EXIT(Result,WakeupSource);
  return Result;
}
#endif /* CANIF_WAKEUP_VALIDATION_API == STD_ON */
#endif /* CANIF_WAKEUP_SUPPORT_API == STD_ON */



#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
/** \brief Report controller Tx confirmation state */
FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_GetTxConfirmationState
  (
    uint8 ControllerId
  )
{
  CanIf_NotifStatusType RetVal = CANIF_NO_NOTIFICATION;   /* return value */

  DBG_CANIF_GETTXCONFIRMATIONSTATE_ENTRY(ControllerId);

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTXCONFIRMSTATE,
                            CANIF_E_UNINIT
                          );
  }

  /* parameter check */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETTXCONFIRMSTATE,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* CANIF734: Return if a Tx confirmation was received since the last
       controller start. */
    if( (CanIf_CanControllerFlags[ControllerId] & CANIF_TX_CONFIRMATION_MASK) != 0U )
    {
      RetVal = CANIF_TX_RX_NOTIFICATION;
    }
  }

  DBG_CANIF_GETTXCONFIRMATIONSTATE_EXIT(RetVal,ControllerId);
  return RetVal;
}
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON) && ( CANIF_TRCV_SUPPORT == STD_ON)
FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ClearTrcvWufFlag
  (
    uint8 TransceiverId
  )
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 TrcvDrvIdx;

  DBG_CANIF_CLEARTRCVWUFFLAG_ENTRY(TransceiverId);
                                                      /* trcv. drv. index */

  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CLEARTRCVWUFFLAG,
                            CANIF_E_UNINIT
                          );
  }

  /* --- perform parameter checks ---------------------------------------- */

  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CLEARTRCVWUFFLAG,
                            CANIF_E_PARAM_TRCV
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].clearWufFlag
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId
               );
  }

  DBG_CANIF_CLEARTRCVWUFFLAG_EXIT(RetVal,TransceiverId);
  return RetVal;
}

FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckTrcvWakeFlag
  (
    uint8 TransceiverId
  )
{
  Std_ReturnType RetVal = E_NOT_OK;
  uint8 TrcvDrvIdx;

  DBG_CANIF_CHECKTRCVWAKEFLAG_ENTRY(TransceiverId);
                                                      /* trcv. drv. index */

  /* --- state check ----------------------------------------------------- */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKTRCVWAKEFLAG,
                            CANIF_E_UNINIT
                          );
  }

  /* --- perform parameter checks ---------------------------------------- */

  /* transceiver ID */
  else if( TransceiverId >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKTRCVWAKEFLAG,
                            CANIF_E_PARAM_TRCV
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  {
    /* retrieve the CanTrcvDrv index from the CanIf TransceiverId */
    TrcvDrvIdx = CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvDrvIdx;

    /* call CanTrcv API function */
    RetVal = CanIf_CanTrcvConfig[TrcvDrvIdx].checkWakeFlag
               (
                 CANIF_CFG_GET_TRCV_CFG(TransceiverId).trcvChnlId
               );
  }

  DBG_CANIF_CHECKTRCVWAKEFLAG_EXIT(RetVal,TransceiverId);
  return RetVal;
}
#endif /* (CANIF_PUBLIC_PN_SUPPORT == STD_ON) &&  CANIF_TRCV_SUPPORT == STD_ON) */


/** \brief Transmit confirmation callback function */
FUNC( void, CANIF_CODE ) CanIf_TxConfirmation( PduIdType CanTxPduId )
{

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
  boolean ClearTxBuffers = TRUE;  /* clear Tx buffer flag */
#endif

  DBG_CANIF_TXCONFIRMATION_ENTRY(CanTxPduId);

  /* --- perform parameter checks ---------------------------------------- */

  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {

#if( CANIF_DEV_ERROR_DETECT == STD_ON )

    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TXCONFIRMATION,
                            CANIF_E_UNINIT
                          );

#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
  else
  {
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    if( CanTxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_TXCONFIRMATION,
                              CANIF_E_PARAM_LPDU
                            );
    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {

#if( ( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON ) ||          \
     ( CANIF_PUBLIC_TX_BUFFERING == STD_ON ) ||                 \
     ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) ||    \
     ( CANIF_PUBLIC_PN_SUPPORT == STD_ON )                      \
   )

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
      const Can_HwHandleType CanHth = CanIf_TxPduConfigPtr[CanTxPduId].hth; /* CAN HTH */
#endif
#if (CANIF_SINGLE_CAN_CTRL_OPT == STD_OFF)
      const uint8 ControllerId = CanIf_TxPduConfigPtr[CanTxPduId].intCtrlIdx; /* controller index */
#else
      const uint8 ControllerId = CANIF_SINGLE_CAN_CTRL_ID; /* controller index */
#endif

      /* --- preparation ----------------------------------------------------- */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
      uint8 ctrlFlags;
      uint8 mirrStatus;
      TS_AtomicAssign8(ctrlFlags, CanIf_CanControllerFlags[ControllerId]);
      TS_AtomicAssign8(mirrStatus, CanIf_MirrorBuff[CanTxPduId].status);
      if((CANIF_BUS_MIRRORING_MASK == (ctrlFlags & CANIF_BUS_MIRRORING_MASK)) &&
          (CANIF_FRAME_READY_TO_MIRROR == mirrStatus)
          )
      {
        /* !LINKSTO CanIf.ASR44.SWS_CANIF_00905.Provision.CANID,1 */
        /* !LINKSTO CanIf.ASR44.SWS_CANIF_00905.Provision.Payload,1 */
        Mirror_ReportCanFrame(ControllerId,
                              CANIF_TRANSLATE_TX_CAN_ID(CanIf_TxPduConfigPtr[(CanTxPduId)].canId),
                              CanIf_MirrorBuff[CanTxPduId].length,
                              &CanIf_MirrorBuffPayload[CanIf_TxPduConfigPtr[CanTxPduId].mirrBuffOfs]
                              );
        mirrStatus = CANIF_FRAME_MIRROR_DONE;
        TS_AtomicAssign8(CanIf_MirrorBuff[CanTxPduId].status, mirrStatus);
      }
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
      /* CANIF751: Disable partial networking filter.
         No check for pNFilterEnabled as configuration access and branch is
         likely to be slower than just doing the operation (resetting the flag is
         allowed within every Tx confirmation). */

      DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])&((uint8)(~CANIF_PN_FILTER_ACTIVE_MASK))));
      CanIf_CanControllerFlags[ControllerId] &= (uint8)(~CANIF_PN_FILTER_ACTIVE_MASK);
#endif /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */

      /* check that current controller and PDU mode allow message transmission */
      if( CanIf_CanControllerMode[ControllerId] == CANIF_CS_STARTED )
      {
#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
        /* CANIF740: Store Tx confirmation flag for the controller */
        if( (CanIf_CanControllerFlags[ControllerId] & CANIF_TX_CONFIRMATION_MASK) == 0U )
        {

          DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])|(CANIF_TX_CONFIRMATION_MASK)));
          CanIf_CanControllerFlags[ControllerId] |= CANIF_TX_CONFIRMATION_MASK;
        }
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

        if( CanIf_PduMode[ControllerId] == CANIF_ONLINE )
        {
#if( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON )

          /* --- set Tx notification flag -------------------------------------- */
          /* get notification index */
          CanIf_LPduIndexType NotifIdx = CanIf_TxPduConfigPtr[CanTxPduId].notifyIndex;
          /* check index */
          if( NotifIdx != CANIF_INVALID_LPDU_INDEX )
          {
            /* set notification flag */
            CANIF_SET_NOTIFICATION_FLAG( CanIf_TxLPduNotifyFlags, NotifIdx );
          }
#endif /* CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON */



#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

          /* don't clear Tx buffers */
          ClearTxBuffers = FALSE;
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */
        }
      }
      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
      if( ClearTxBuffers == TRUE )
      {
        /* clear Tx buffers for this HTH */
        CanIf_ClearHthTxRequests( CanHth, ControllerId );
      }
      else
      {
        /* --- trigger transmission of next pending Tx request --------------- */
        /* call function to trigger next Tx request */
        CanIf_SendNextTxRequest( CanHth, ControllerId );
      }
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */
#endif /* ( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON ) || \
          ( CANIF_PUBLIC_TX_BUFFERING == STD_ON ) || \
          ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) || \
          ( CANIF_PUBLIC_PN_SUPPORT == STD_ON ) */


      /* --- call upper layer confirmation function -------------------------- */

#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
  #if(CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON)
      /* check if decoupled processing is enabled */
      if(CanIf_TxPduConfigPtr[CanTxPduId].MFIndex != CANIF_INVALID_MF_INDEX)
      {
        uint8 MFIdx = CanIf_TxPduConfigPtr[CanTxPduId].MFIndex;

        if(E_NOT_OK != CanIf_InsertTxQueue(CanTxPduId))
        {
          /* forward the call to SchM */
          /* !LINKSTO CanIf.EB.DecoupledProcessing.TriggerMainFunc,1 */
          (CanIf_MfTxFctPtrTbl[MFIdx].ActMainTxFctPtr)();
        }
#if((CANIF_DEV_ERROR_DETECT == STD_ON) || (CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON))
        else
        {
#if(CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
          CanIf_NoOfTxPdusExceedingQueue[MFIdx]++;

          /* !LINKSTO CanIf.EB.DecoupledMeasurement.NumberOfTxPdusExceedingQueueApiName,1 */
          /* forward to CDD the number of Tx Pdus that could not be added in the queue */
          CANIF_TX_EXCEED_QUEUE_CALLBACK(MFIdx, CanIf_NoOfTxPdusExceedingQueue[MFIdx]);
#endif /* CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */

#if(CANIF_DEV_ERROR_DETECT == STD_ON)
          /* report DET error */
          /* !LINKSTO CanIf.EB.TxDecoupledProcessing.FullQueue,1 */
          CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
              CANIF_INSTANCE_ID,
              CANIF_SID_TXCONFIRMATION,
              CANIF_E_FULL_QUEUE
          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
        }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON || CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
      }
      else
  #endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON */
      {
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
        CanIf_SendTxConfirmation( CanTxPduId, E_OK);
#else /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON  */
        CanIf_SendTxConfirmation( CanTxPduId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
      }
#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */
    }
  }

  DBG_CANIF_TXCONFIRMATION_EXIT(CanTxPduId);
}



/** \brief Receive indication callback function */
#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
FUNC( void, CANIF_CODE ) CanIf_RxIndication
  (
    Can_HwHandleType Hrh,
    Can_IdType CanId,
    uint8 CanDlc,
    P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtr
  )
#else
FUNC( void, CANIF_CODE ) CanIf_RxIndication
  (
    P2CONST( Can_HwType, AUTOMATIC, CANIF_APPL_DATA ) Mailbox,
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */
{
  /* --- state and parameter checks -------------------------------------- */

  /* initialization */

#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
  DBG_CANIF_RXINDICATION_ENTRY(Hrh,CanId,CanDlc,CanSduPtr);
#else
  DBG_CANIF_RXINDICATION_ENTRY(Mailbox,PduInfoPtr);
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */

  if( CanIf_State == CANIF_UNINITIALIZED )
  {

#if( CANIF_DEV_ERROR_DETECT == STD_ON )

    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_RXINDICATION,
                            CANIF_E_UNINIT
                          );

#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  }
  else
  {
#if (!defined CANIF_CAN_ASR402_COMPATIBILITY) && (!defined CANIF_CAN_ASR403_COMPATIBILITY)
  #if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* parameter Mailbox */
    if( Mailbox == NULL_PTR )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_RXINDICATION,
                              CANIF_E_PARAM_POINTER
                            );
    }
    else
  #endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
#endif /* (!defined CANIF_CAN_ASR402_COMPATIBILITY) && (!defined CANIF_CAN_ASR403_COMPATIBILITY) */
    {
#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
      const Can_HwHandleType HrhLcl = Hrh;
      const Can_IdType CanIdLcl = CanId;
      const PduLengthType LclDlc = CanDlc;
      P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtrLcl = CanSduPtr;
#else
      const Can_HwHandleType HrhLcl = Mailbox->Hoh;
      const Can_IdType CanIdLcl = Mailbox->CanId;
      const uint8 ControllerId = Mailbox->ControllerId;
      const PduLengthType LclDlc = PduInfoPtr->SduLength;
      P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtrLcl = PduInfoPtr->SduDataPtr;
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
      if(CanIf_RxIndication_DetChecks(HrhLcl,CanIdLcl,LclDlc,CanSduPtrLcl) == E_OK)
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
      {
        /* internal HRH index */
#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
        const uint16 CanIfHrhIdx = CANIF_GET_HRH_INDEX(HrhLcl);
#else
        const uint16 CanIfHrhIdx = (uint16)CANIF_GET_HRH_INDEX((uint32)HrhLcl + CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].HohIdOffset);
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */

        /* --- CanId translation ------------------------------------- */
#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
        const uint8 ControllerId = CANIF_GET_HOH_INT_CTRL_IDX(CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx));
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */

        const Can_IdType ConfCanId = CANIF_TRANSLATE_RX_CAN_ID(CanIdLcl);

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
        uint8 CtrlStatus;
        TS_AtomicAssign8(CtrlStatus, CanIf_CanControllerFlags[ControllerId]);
        if(CANIF_BUS_MIRRORING_MASK == (CtrlStatus & CANIF_BUS_MIRRORING_MASK))
        {
          /* !LINKSTO CanIf.ASR44.SWS_CANIF_00906.Provision.CANID,1 */
          /* !LINKSTO CanIf.ASR44.SWS_CANIF_00906.Provision.Payload,1 */
          Mirror_ReportCanFrame(ControllerId, ConfCanId, (uint8)LclDlc, CanSduPtrLcl);
        }
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
#if( (CANIF_TL_RX_CAN_ID_SUPPORT == STD_ON) && (CANIF_DEV_ERROR_DETECT == STD_ON) )
        /* check the translated CanId */
        if( CANIF_CHECK_FOR_INVALID_CAN_ID( ConfCanId ) )
        {
          /* report DET error */
          CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
              CANIF_INSTANCE_ID,
              CANIF_SID_RXINDICATION,
              CANIF_E_PARAM_CANID
          );
        }
        else
#endif /* (CANIF_TL_RX_CAN_ID_SUPPORT == STD_ON) && (CANIF_DEV_ERROR_DETECT == STD_ON) */
        {
          /* --- software filtering ------------------------------------- */
#if (CANIF_SW_FILTER == STD_ON)
          CanIf_LPduIndexType Index = CANIF_INVALID_LPDU_INDEX; /* Rx L-PDU index in sorted array */
          const Std_ReturnType SwFilterResult = CanIf_SoftwareFiltering( CanIfHrhIdx, ConfCanId, &Index );
          if( SwFilterResult == E_OK )
          {
#else
          CanIf_LPduIndexType Index = CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx).firstIndex; /* Rx L-PDU index in sorted array */
          if(Index != CANIF_INVALID_LPDU_INDEX)
          {
#endif /* CANIF_SW_FILTER == STD_ON */
            const CanIf_LPduIndexType PduIdx = CANIF_CFG_GET_SORTEDRXLPDU_CFG(Index).rxLPduIndex;

            PduInfoType RxPduInfo; /* PDU information */

            RxPduInfo.SduLength = LclDlc;

            /* For performance reasons CanSduPtr shall be given to the upper layer
             without copying its content. An explicit cast is necessary because
             the type definition of PduInfoType contains a P2VAR element for the
             message data. Please also compare the last paragraph in the SWS
             (V4.1.0) in chapter 7.14. */
            /* Deviation MISRAC2012-1 */
            /* Deviation MGCC62-1 */
            RxPduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA)) CanSduPtrLcl;

#if (CANIF_METADATA_SUPPORT == STD_ON)
              /* Check if the Pdu has Metadata.  */
              if (CANIF_CFG_GET_SORTEDRXLPDU_CFG(Index).setMetadataPduId != CANIF_INVALID_LPDU_INDEX)
              {
                PduInfoType MetadataPduInfo; /* Not used by EcuC but needed in the call */

                /* Set the metadata in EcuC. */
                /* !LINKSTO CanIf.EB.Metadata.Functional.3, 1 */
                (void)EcuC_SetMetaDataCanId(CANIF_CFG_GET_SORTEDRXLPDU_CFG(Index).setMetadataPduId, &MetadataPduInfo, CanIdLcl);
              }
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */

            {
#if(CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
              /* set Rx L-PDU config pointer */
              CONSTP2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST ) RxLPduConfig =
                  &(CanIf_RxPduConfigPtr[PduIdx]);
              uint8 MFIdx = RxLPduConfig->MFIndex;

              /* check if decoupled processing is enabled */
              if(MFIdx != CANIF_INVALID_MF_INDEX)
              {
                /* add Pdu to queue */
                if(E_NOT_OK != CanIf_InsertRxQueue(&RxPduInfo, CanIdLcl, PduIdx, ControllerId))
                {
                  /* forward the call to SchM */
                  /* !LINKSTO CanIf.EB.DecoupledProcessing.TriggerMainFunc,1 */
                  (CanIf_MfRxFctPtrTbl[MFIdx].ActMainRxFctPtr)();
                }
#if((CANIF_DEV_ERROR_DETECT == STD_ON) || (CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON))
                else
                {
#if(CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
                  CanIf_NoOfRxPdusExceedingQueue[MFIdx]++;

                  /* !LINKSTO CanIf.EB.DecoupledMeasurement.NumberOfRxPdusExceedingQueueApiName,1 */
                  /* forward to CDD the number of Rx Pdus that could not be added in the queue */
                  CANIF_RX_EXCEED_QUEUE_CALLBACK(MFIdx, CanIf_NoOfRxPdusExceedingQueue[MFIdx]);
#endif /* CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */

#if(CANIF_DEV_ERROR_DETECT == STD_ON)
                  /* report DET error */
                  /* !LINKSTO CanIf.EB.RxDecoupledProcessing.FullQueue,1 */
                  CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                      CANIF_INSTANCE_ID,
                      CANIF_SID_RXINDICATION,
                      CANIF_E_FULL_QUEUE
                  );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
                }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON || CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
              }
              else
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */
              {
                CanIf_ProcessRxPdu(&RxPduInfo, ConfCanId, PduIdx, ControllerId);
              }
            }
          }
        }
      }
    }
  }

#if(defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
  DBG_CANIF_RXINDICATION_EXIT(Hrh,CanId,CanDlc,CanSduPtr);
#else
  DBG_CANIF_RXINDICATION_EXIT(Mailbox,PduInfoPtr);
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */

}


#if(CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON)
/** \brief Rx main function */
/* !LINKSTO CanIf.EB.RxDecoupledProcessing.MainFunc,1 */
FUNC( void, CANIF_CODE ) CanIf_MainFunctionRx_Generic
(
  uint8 Idx
)
{
  /* only do something in state CANIF_INITIALIZED */
  if( CanIf_State == CANIF_INITIALIZED )
  {
    uint16 i;
    uint8 Data[CANIF_MAXIMUM_CAN_FD_DLC];
    const CanIf_MfRxPtrTblType MfTbl = CanIf_MfRxFctPtrTbl[Idx];
    const uint16 StartIdx = MfTbl.RxQueueOffset;
    const uint16 StopIdx = (uint16)MfTbl.RxQueueOffset + (uint16)MfTbl.RxQueueSize;

#if(CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    uint16 RxQueueSize;

    TS_AtomicAssign16(RxQueueSize, CanIf_TotalRxQueueSize[Idx]);

    /* !LINKSTO CanIf.EB.DecoupledMeasurement.NumberOfEnqueuedRxPdusApiName,1 */
    /* Forward to CDD the number of enqueued Pdus */
    CANIF_ENQUEUED_RXPDUS_CALLBACK(Idx, RxQueueSize);
#endif /* CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */

    /* !LINKSTO CanIf.EB.RxDecoupledProcessing,1 */
    /* The loop gets broken out of at the end, condition is always true */
    /* CHECK: NOPARSE */
    for(i = StartIdx; i < StopIdx; i++)
    /* CHECK: PARSE */
    {
      CanIf_RxQueueType rxQueue;
      PduInfoType RxPduInfo; /* PDU information */
      boolean processPdu = FALSE;
      boolean processFinished = FALSE;

      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      rxQueue = CanIf_RxPduQueue[i];

      if(0U == CanIf_TotalRxQueueSize[Idx])
      {
        /* queue is empty */
        processFinished = TRUE;
      }

      if(rxQueue.MFIdx == Idx)
      {
        /* get the saved payload from queue */
        TS_MemCpy(&Data[0U], &CanIf_RxDataQueue[rxQueue.PreviousQueuePos], rxQueue.PayloadLen);

        RxPduInfo.SduLength = rxQueue.PayloadLen;
        RxPduInfo.SduDataPtr = Data;

        /* clear the MainFunction index */
        CanIf_RxPduQueue[i].MFIdx = CANIF_INVALID_MF_INDEX;
        processPdu = TRUE;

        if((rxQueue.PreviousQueuePos + rxQueue.PayloadLen) == CanIf_MFRxQueueIdx[Idx])
        {
          processFinished = TRUE;
        }
      }

      if(TRUE == processFinished)
      {
        /* remove Pdu from queue */
        CanIf_TotalRxQueueSize[Idx] = 0U;
        /* clear the index of the payload data */
        CanIf_MFRxQueueIdx[Idx] = CanIf_MfRxFctPtrTbl[Idx].PayloadQueueOffset;
      }

      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      /* if PDU was added to queque, process it */
      if(TRUE == processPdu)
      {
        CanIf_ProcessRxPdu(&RxPduInfo, rxQueue.CanId, rxQueue.PduId, rxQueue.ControllerId);
      }

      if(TRUE == processFinished)
      {
        break;
      }
    }

#if(CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    /* clear the Rx buffer for Pdus that exceeded the configured queue size */
    TS_AtomicAssign16(CanIf_NoOfRxPdusExceedingQueue[Idx], 0U);
#endif /* CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
  }
}
#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */


#if(CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON)
/** \brief Tx main function */
/* !LINKSTO CanIf.EB.TxDecoupledProcessing.MainFunc,1 */
FUNC( void, CANIF_CODE ) CanIf_MainFunctionTx_Generic
(
  uint8 Idx
)
{
  /* only do something in state CANIF_INITIALIZED */
  if( CanIf_State == CANIF_INITIALIZED )
  {
    sint32 i;
    const CanIf_MfTxPtrTblType MfTbl = CanIf_MfTxFctPtrTbl[Idx];
    const uint16 StartIdx = MfTbl.TxQueueOffset;
    const uint16 StopIdx = (uint16)MfTbl.TxQueueOffset + (uint16)MfTbl.TxQueueSize;

#if(CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    uint16 TxQueueSize;

    TS_AtomicAssign16(TxQueueSize, CanIf_TotalTxQueueSize[Idx]);

    /* !LINKSTO CanIf.EB.DecoupledMeasurement.NumberOfEnqueuedTxPdusApiName,1 */
    /* Forward to CDD the number of enqueued Pdus */
    CANIF_ENQUEUED_TXPDUS_CALLBACK(Idx, TxQueueSize);
#endif /* CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */

    while( 0U != CanIf_TotalTxQueueSize[Idx] )
    {
      /* !LINKSTO CanIf.EB.TxDecoupledProcessing,1 */
      /* Process the queue backwards because it's a FILO */
      for(i = StopIdx - 1; i >= StartIdx; i--)
      {
        boolean processPdu = FALSE;
        PduIdType TxPduId;

        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* store the saved PduId */
        TxPduId = CanIf_TxPduQueue[i].PduId;

        if(Idx == CanIf_TxPduQueue[i].MFIdx)
        {
          /* clear the MainFunction index */
          CanIf_TxPduQueue[i].MFIdx = CANIF_INVALID_MF_INDEX;

          /* Decrease the queue size. */
          CanIf_TotalTxQueueSize[Idx]--;

          processPdu = TRUE;
        }

        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* if PDU was added to queue, process it */
        if(TRUE == processPdu)
        {
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
          CanIf_SendTxConfirmation( TxPduId, E_OK);
#else /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON  */
          CanIf_SendTxConfirmation( TxPduId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
        }
      }
    }

#if(CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    /* clear the Tx buffer for Pdus that exceeded the configured queue size */
    TS_AtomicAssign16(CanIf_NoOfTxPdusExceedingQueue[Idx], 0U);
#endif /* CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
  }
}
#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON */


#if( CANIF_CANCEL_TX_CONFIRMATION_CB == STD_ON )
/** \brief Transmit cancellation confirmation callback function */
#ifndef CANIF_CAN_ASR402_COMPATIBILITY
FUNC( void, CANIF_CODE ) CanIf_CancelTxConfirmation
  (
    PduIdType CanTxPduId,
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
#else
FUNC( void, CANIF_CODE ) CanIf_CancelTxConfirmation
  (
    P2CONST( Can_PduType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
#endif
{

  /* --- state and parameter checks -------------------------------------- */

  /* initialization */

#if (defined CANIF_CAN_ASR402_COMPATIBILITY)
  DBG_CANIF_CANCELTXCONFIRMATION_ENTRY(PduInfoPtr);
#else
  DBG_CANIF_CANCELTXCONFIRMATION_ENTRY(CanTxPduId, PduInfoPtr);
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */

  if( CanIf_State == CANIF_UNINITIALIZED )
  {

#if( CANIF_DEV_ERROR_DETECT == STD_ON )

    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CANCELTXCONFIRMATION,
                            CANIF_E_UNINIT
                          );

#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
  else
  {

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* check for NULL pointers */
    if( PduInfoPtr == NULL_PTR )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                              CANIF_INSTANCE_ID,
                              CANIF_SID_CANCELTXCONFIRMATION,
                              CANIF_E_PARAM_POINTER
                            );
    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {


      /* initialize variables dependent on API signature and after(!) PduInfoPtr check */
#if (defined CANIF_CAN_ASR402_COMPATIBILITY)
      const uint8 SduLength = PduInfoPtr->length;  /* CAN data length */
      /* transmit L-PDU ID */
      const CanIf_LPduIndexType TxPduId =
        (CanIf_LPduIndexType)(PduInfoPtr->swPduHandle);
      /* Pointer to CAN data */
      CONSTP2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr = PduInfoPtr->sdu;
#else /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */
      const PduLengthType SduLength = PduInfoPtr->SduLength;  /* CAN data length */
      /* transmit L-PDU ID */
      const CanIf_LPduIndexType TxPduId = (CanIf_LPduIndexType) CanTxPduId;
      /* Pointer to CAN data */
      CONSTP2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr = PduInfoPtr->SduDataPtr;
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
      if( SduDataPtr == NULL_PTR )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_CANCELTXCONFIRMATION,
                                CANIF_E_PARAM_POINTER
                              );
      }
#if (defined CANIF_CAN_ASR402_COMPATIBILITY)
      /* check CAN ID */
      else if( CANIF_CHECK_FOR_INVALID_CAN_ID( PduInfoPtr->id ) )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_CANCELTXCONFIRMATION,
                                CANIF_E_PARAM_CANID
                              );
      }
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */
      /* check DLC */
      else if( SduLength > CANIF_MAXIMUM_CAN_FD_DLC )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_CANCELTXCONFIRMATION,
                                CANIF_E_PARAM_DLC
                              );
      }
      else if( TxPduId >= CanIf_ConfigPtr->nrOfTxLPdus )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_CANCELTXCONFIRMATION,
                                CANIF_E_PARAM_LPDU
                              );
      }
      else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {

#if (CANIF_SINGLE_CAN_CTRL_OPT ==STD_OFF)
        const uint8 ControllerId = CanIf_TxPduConfigPtr[TxPduId].intCtrlIdx;                     /* Controller index */
#else
        const uint8 ControllerId = CANIF_SINGLE_CAN_CTRL_ID;                                     /* Controller index */
#endif

        const Can_HwHandleType CanHth = CanIf_TxPduConfigPtr[TxPduId].hth;                       /* CAN HTH */
        const Can_HwHandleType CanHthIdx = (Can_HwHandleType) CANIF_GET_HTH_INDEX(CanHth + CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].HohIdOffset);       /* CAN HTH index */

        boolean send_request = FALSE;

        /* --- trigger next transmission --------------------------------------- */

        /* enter critical section */
        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* check if buffering the message is allowed at all */
        if( ( CanIf_CanControllerMode[ControllerId] == CANIF_CS_STARTED ) &&
            (CanIf_PduMode[ControllerId] == CANIF_ONLINE)
          )
        {
          /* --- try to buffer old message ------------------------------------- */

          CanIf_TxBufferIndexType BufIdx; /* Tx buffer index */

          /* request Tx buffer */
          BufIdx = CANIF_GET_TX_BUFFER_INDEX( TxPduId );

          /* check if buffering is allowed for this Tx PDU */
          if( BufIdx != CANIF_INVALID_TX_BUFFER_INDEX )
          {
            /* check if L-PDU is currently buffering a new Tx request */
            if( CANIF_TX_BUFFER_IS_EMPTY( BufIdx ) == TRUE )
            {
              const CanIf_TxBufferIndexType memIndex = CanIf_TxPduConfigPtr[TxPduId].memIndex;
              /* write canceled message to Tx buffer */
#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )
              {
                Can_IdType CanId;   /* CAN Identifier */
#if (defined CANIF_CAN_ASR402_COMPATIBILITY)
                CanId = PduInfoPtr->id;
#else /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */
                /*CAN Identifier not provided from API, read it from configuration */
                CanId = (TxPduId >= CanIf_ConfigPtr->nrOfStaticTxLPdus)
                        ? CanIf_DynamicTxLPduCanIds[TxPduId - CanIf_ConfigPtr->nrOfStaticTxLPdus]
                        : CanIf_TxPduConfigPtr[TxPduId].canId;
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */
                CanIf_TxBuffer[BufIdx].id = CanId;
              }
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */
              CanIf_TxBuffer[BufIdx].length = (uint8)SduLength;
              TS_MemCpy(&CanIf_TxBufferMemory[memIndex], SduDataPtr, SduLength);

              /* add L-PDU to list of pending Tx requests */
              CanIf_AddPendingTxRequest( CanHthIdx, TxPduId );
            }
            /* else: cancelled message is discarded (CANIF176) because buffer is
                     already filled */
          }
          /* else : cancelled message is lost */



          /* --- trigger transmission of next Tx request ----------------------- */
          send_request = TRUE;
        }

        /* leave critical section */
        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        if(send_request == TRUE)
        {
          /* call function to trigger next Tx request */
          CanIf_SendNextTxRequest( CanHth, ControllerId );
        }
        else
        {
          /* clear Tx buffers for this HTH */
          CanIf_ClearHthTxRequests( CanHth, ControllerId );
        }
      }
    }
  }

#if (defined CANIF_CAN_ASR402_COMPATIBILITY)
  DBG_CANIF_CANCELTXCONFIRMATION_EXIT(PduInfoPtr);
#else
  DBG_CANIF_CANCELTXCONFIRMATION_EXIT(CanTxPduId, PduInfoPtr);
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) */
}
#endif /* CANIF_CANCEL_TX_CONFIRMATION_CB == STD_ON */



/** \brief Bus-off callback function */
FUNC( void, CANIF_CODE ) CanIf_ControllerBusOff( uint8 Controller )
{

  DBG_CANIF_CONTROLLERBUSOFF_ENTRY(Controller);

  /* --- perform parameter checks ---------------------------------------- */

  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* CANIF431: Abort if module is not initialized */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERBUSOFF,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  /* parameter Controller */
#if( CANIF_SINGLE_CAN_CTRL_OPT == STD_ON )
  else if( Controller != CANIF_SINGLE_CAN_CTRL_ID )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERBUSOFF,
                            CANIF_E_PARAM_CONTROLLER
                          );
  }
#else
  else if( Controller >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERBUSOFF,
                            CANIF_E_PARAM_CONTROLLER
                          );
  }
#endif /* CANIF_SINGLE_CAN_CTRL_OPT == STD_ON */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

  else
  {
    /* module is initialized and no Det found till now */

#if( CANIF_DEV_ERROR_DETECT != STD_ON )
    /* prevent ISO-C warning for overwriting parameter without using it */
    TS_PARAM_UNUSED( Controller );
#endif /* CANIF_DEV_ERROR_DETECT != STD_ON */

    /* --- change mode of current controller ----------------------------- */

    /* set controller mode to STOPPED */

    DBG_CANIF_CANCONTROLLERMODE_GRP(Controller,(CanIf_CanControllerMode[ControllerId]),(CANIF_CS_STOPPED));
    CanIf_CanControllerMode[Controller] = CANIF_CS_STOPPED;


    /* --- SWS_CANIF_00866: set pdu channel mode to CANIF_TX_OFFLINE --- */
    CanIf_PduMode[Controller] = CANIF_TX_OFFLINE;

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

    /* no transmissions allowed -> clear Tx message buffers */
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    /* !LINKSTO CanIf.EB.SWS_CANIF_00739.SendTxConf_NotOk, 1 */
    CanIf_FreeControllerTxBuffers( Controller, TRUE );
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    CanIf_FreeControllerTxBuffers( Controller );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
    /* CANIF739: Clear Tx confirmation flag in case of a controller mode is CANIF_CS_STOPPED */
    /* !LINKSTO CanIf.EB.SWS_CANIF_00739.ClearTxConfFlag, 1 */
    CanIf_ClearTxConfirmationFlag(Controller);
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    CanIf_ResetMirrBuff(Controller);
#endif

    /* --- call upper layer callback function ---------------------------- */

    /* call configured bus-off notification function */
    CANIF_BUSOFF_CALLBACK( Controller );
  }

  DBG_CANIF_CONTROLLERBUSOFF_EXIT(Controller);
}



/** \brief Controller mode indication callback function */
FUNC( void, CANIF_CODE ) CanIf_ControllerModeIndication
  (
    uint8 ControllerId,
#if (defined CANIF_CAN_ASR431_COMPATIBILITY) || (defined CANIF_CAN_ASR440_COMPATIBILITY)
    Can_ControllerStateType ControllerMode
#else
    CanIf_ControllerModeType ControllerMode
#endif /* (defined CANIF_CAN_ASR431_COMPATIBILITY) || (defined CANIF_CAN_ASR440_COMPATIBILITY) */
  )
{
  DBG_CANIF_CONTROLLERMODEINDICATION_ENTRY(ControllerId,ControllerMode);
  /* --- perform parameter checks ---------------------------------------- */

#if( (CANIF_DEV_ERROR_DETECT == STD_OFF) && (CANIF_SINGLE_CAN_CTRL_OPT == STD_ON) )
  /* prevent compiler warning for unused parameter */

  TS_PARAM_UNUSED( ControllerId );
#endif /* (CANIF_DEV_ERROR_DETECT == STD_OFF) && (CANIF_SINGLE_CAN_CTRL_OPT == STD_ON) */

  /* initialization check */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {

#if( CANIF_DEV_ERROR_DETECT == STD_ON )

    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERMODEIND,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
#if( CANIF_SINGLE_CAN_CTRL_OPT == STD_ON )
  /* parameter ControllerId */
  else if( ControllerId != CANIF_SINGLE_CAN_CTRL_ID )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERMODEIND,
                            CANIF_E_PARAM_CONTROLLER
                          );
  }
#else /* CANIF_SINGLE_CAN_CTRL_OPT == STD_ON */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERMODEIND,
                            CANIF_E_PARAM_CONTROLLER
                          );
  }
#endif /* CANIF_SINGLE_CAN_CTRL_OPT == STD_ON */
  else if(    ((CanIf_ControllerModeType)ControllerMode != CANIF_CS_SLEEP)
           && ((CanIf_ControllerModeType)ControllerMode != CANIF_CS_STARTED)
           && ((CanIf_ControllerModeType)ControllerMode != CANIF_CS_STOPPED)
         )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONTROLLERMODEIND,
                            CANIF_E_PARAM_CTRLMODE
                          );

  }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  else /* initialization check and DET checks have been passed by */
  {

#if( ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
     ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) )
    /* mask, which ControllerId flags shall be cleared */
    uint8 CtrlModeClearFlagMask = 0U;
#endif /* ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
          ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) */

#if( ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
     ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) )
    switch((CanIf_ControllerModeType)ControllerMode)
    {
      case CANIF_CS_STOPPED:
        break;

      case CANIF_CS_SLEEP:
      /* reset wakeup validation flag if changed to sleep mode */
        CtrlModeClearFlagMask = CANIF_WAKEUP_VALIDATION_MASK;

#if( CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF )
        /* reset wakeup validation flag if changed to sleep mode */
        CtrlModeClearFlagMask |= CANIF_WAKEUP_MASK;
#endif /* CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF */
        break;

      case CANIF_CS_STARTED:
#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
        /* CANIF734: Combined Tx confirmation state shall be summarized since
           last ControllerId start. */
        if( CanIf_CanControllerMode[ControllerId] != CANIF_CS_STARTED )
        {
          CtrlModeClearFlagMask = CANIF_TX_CONFIRMATION_MASK;
        }
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */
        break;

      /* CHECK: NOPARSE */
      default:
        /* this may not happen if new mode was checked as expected */
        CANIF_UNREACHABLE_CODE_ASSERT(CANIF_SID_CONTROLLERMODEIND);
        break;
      /* CHECK: PARSE */
    }
#endif /* ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
        ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) */

    /* enter critical section */
    SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

    /* set new controller mode */

    DBG_CANIF_CANCONTROLLERMODE_GRP(ControllerId,(CanIf_CanControllerMode[ControllerId]),(ControllerMode));
    CanIf_CanControllerMode[ControllerId] = (CanIf_ControllerModeType)ControllerMode;

#if( ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
     ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) )
    /* clear pending flag and state specific mode flags */

    DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])&((uint8)(~CtrlModeClearFlagMask))));
    CanIf_CanControllerFlags[ControllerId] &= (uint8)(~CtrlModeClearFlagMask);
#endif /* ( CANIF_WAKEUP_VALIDATION_API == STD_ON ) || \
          ( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON ) */

    /* exit critical section */
    SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#if(( CANIF_PUBLIC_TX_BUFFERING == STD_ON ) || (CANIF_BUS_MIRRORING_SUPPORT == STD_ON))
    if((CanIf_ControllerModeType)ControllerMode == CANIF_CS_STOPPED)
    {
      /* no transmissions allowed -> clear Tx message buffers */
#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
    /* no transmissions allowed -> clear Tx message buffers */
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    /* !LINKSTO CanIf.EB.SWS_CANIF_00739.SendTxConf_NotOk, 1 */
    CanIf_FreeControllerTxBuffers( ControllerId, TRUE );
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    CanIf_FreeControllerTxBuffers( ControllerId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
    /* CANIF739: Clear Tx confirmation flag in case of a controller mode is CANIF_CS_STOPPED */
    /* !LINKSTO CanIf.EB.SWS_CANIF_00739.ClearTxConfFlag, 1 */
    CanIf_ClearTxConfirmationFlag(ControllerId);
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

#if( CANIF_BUS_MIRRORING_SUPPORT == STD_ON )
      CanIf_ResetMirrBuff( ControllerId );
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

    }
#endif /* ( CANIF_PUBLIC_TX_BUFFERING == STD_ON ) || (CANIF_BUS_MIRRORING_SUPPORT == STD_ON) */

    /* call configured controller mode change indication function */
    CANIF_CTRLMODEINDICATION_CALLBACK( ControllerId, (CanIf_ControllerModeType)ControllerMode);
  }

  DBG_CANIF_CONTROLLERMODEINDICATION_EXIT(ControllerId,ControllerMode);
}



#if( CANIF_TRCV_SUPPORT == STD_ON )

/** \brief Transceiver mode indication callback function */
FUNC( void, CANIF_CODE ) CanIf_TrcvModeIndication
  (
    uint8 Transceiver,
    CanTrcv_TrcvModeType TransceiverMode
  )
{

  DBG_CANIF_TRCVMODEINDICATION_ENTRY(Transceiver,TransceiverMode);

  /* --- state check ----------------------------------------------------- */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* initialization check */
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSCEIVERMODEIND,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  else if( Transceiver >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSCEIVERMODEIND,
                            CANIF_E_PARAM_TRCV
                          );
  }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  else
  {
    CANIF_TRCVMODEINDICATION_CALLBACK(Transceiver, TransceiverMode);
  }

  DBG_CANIF_TRCVMODEINDICATION_EXIT(Transceiver,TransceiverMode);
}

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )

FUNC( void, CANIF_CODE ) CanIf_ConfirmPnAvailability
  (
    uint8 Transceiver
  )
{
  /* --- state check ----------------------------------------------------- */

  DBG_CANIF_CONFIRMPNAVAILABILITY_ENTRY(Transceiver);

  if( CanIf_State == CANIF_UNINITIALIZED )
  {
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* initialization check */
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONFIRMPNAVAILABILITY,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  else if( Transceiver >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CONFIRMPNAVAILABILITY,
                            CANIF_E_PARAM_TRCV
                          );
  }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  else
  {
    CANIF_CONFIRMPNAVAILABILITY_CALLBACK(Transceiver);
  }

  DBG_CANIF_CONFIRMPNAVAILABILITY_EXIT(Transceiver);
}

FUNC( void, CANIF_CODE ) CanIf_ClearTrcvWufFlagIndication
  (
    uint8 Transceiver
  )
{
  /* --- state check ----------------------------------------------------- */

  DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_ENTRY(Transceiver);

  if( CanIf_State == CANIF_UNINITIALIZED )
  {
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* initialization check */
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CLEARTRCVWUFFLAGIND,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  else if( Transceiver >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CLEARTRCVWUFFLAGIND,
                            CANIF_E_PARAM_TRCV
                          );
  }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  else
  {
    CANIF_CLEARTRCVWUFFLAGINDICATION_CALLBACK(Transceiver);
  }

  DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_EXIT(Transceiver);
}

FUNC( void, CANIF_CODE ) CanIf_CheckTrcvWakeFlagIndication
  (
    uint8 Transceiver
  )
{
  /* --- state check ----------------------------------------------------- */

  DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_ENTRY(Transceiver);

  if( CanIf_State == CANIF_UNINITIALIZED )
  {
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    /* initialization check */
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKTRCVWAKEFLAGIND,
                            CANIF_E_UNINIT
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  }
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
  else if( Transceiver >= CanIf_ConfigPtr->nrOfTransceivers )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_CHECKTRCVWAKEFLAGIND,
                            CANIF_E_PARAM_TRCV
                          );
  }
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  else
  {
    CANIF_CHECKTRCVWAKEFLAGINDICATION_CALLBACK(Transceiver);
  }

  DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_EXIT(Transceiver);
}
#endif /* #if CANIF_PUBLIC_PN_SUPPORT == STD_ON */

#endif /*  CANIF_TRCV_SUPPORT == STD_ON */



#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>



/*==================[internal function definitions]==========================*/

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

/** \brief Set mode of a CAN controller */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetControllerModeInternal
  (
    uint8 ControllerId,
    CanIf_ControllerModeType ControllerMode
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  uint8 canIdx;
  uint8 canCtrlId;

#if( CANIF_SINGLE_CAN_CTRL_OPT == STD_ON )
  /* prevent ISO-C warning for overwriting parameter without using it */
  TS_PARAM_UNUSED( ControllerId );
  /* force parameter ControllerId to single controller ID #define to aid
     compiler optimization */
  ControllerId = CANIF_SINGLE_CAN_CTRL_INDEX;
#endif /* CANIF_SINGLE_CAN_CTRL_OPT == STD_ON */

  canIdx = CANIF_GET_CAN_DRIVER_IDX(ControllerId);
  canCtrlId = CANIF_GET_CANDRV_CTRL_ID(ControllerId);

  /* check requested mode */
  switch( ControllerMode )
  {
    case CANIF_CS_STOPPED:
    {
      /* SWS_CANIF_00866: set pdu channel mode to CANIF_TX_OFFLINE */
      CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE;

      /* !LINKSTO CanIf.EB.CANIF487, 1 */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      /* for stopped, we must choose different commandos, depending on the current state */
      if(CanIf_CanControllerMode[ControllerId] == CANIF_CS_SLEEP)
      {
        /* Call Can_SetControllerMode() to wakeup the can controller */
        if (CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, CANIF_T_WAKEUP ) == CAN_OK)
        {
          RetVal = E_OK;
        }
      }
      else
      {
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */

        /* Call Can_SetControllerMode() to stop the can controller */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
        if (CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, CANIF_T_STOP ) == CAN_OK)
#else
        /* cast to uint8 is done because casting an enum type to another is raising warnings, even if the two types are compatible */
        if ((Can_ReturnType)(uint8)CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, (Can_ControllerStateType)CANIF_T_STOP ) == CAN_OK)
#endif
        {
          RetVal = E_OK;
        }
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      }
#endif /* (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY) */
    }
    break;

    case CANIF_CS_STARTED:
    {
      /* Call Can_SetControllerMode() to start the can controller */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      if (CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, CANIF_T_START ) == CAN_OK)
#else
      /* cast to uint8 is done because casting an enum type to another is raising warnings, even if the two types are compatible */
      if ((Can_ReturnType)(uint8)CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, (Can_ControllerStateType)CANIF_T_START ) == CAN_OK)
#endif
      {
        RetVal = E_OK;
      }
    }
    break;

    case CANIF_CS_SLEEP:
    {

      /* Call Can_SetControllerMode() to put the can controller into sleep mode */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      if (CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, CANIF_T_SLEEP ) == CAN_OK)
#else
      /* cast to uint8 is done because casting an enum type to another is raising warnings, even if the two types are compatible */
      if ((Can_ReturnType)(uint8)CanIf_CanDrvFctConfig[canIdx].SetControllerMode( canCtrlId, (Can_ControllerStateType)CANIF_T_SLEEP ) == CAN_OK)
#endif
      {
        RetVal = E_OK;
      }

      /* SWS_CANIF_00865: set pdu mode to CANIF_CS_SLEEP */
      CanIf_PduMode[ControllerId] = CANIF_OFFLINE;           /* reset Pdu mode when can controller is set to CANIF_CS_SLEEP */

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )

     /* SWS_CANIF_00749: Enable partial networking filter when controllerMode is requested to CANIF_CS_SLEEP */
      if( CANIF_CFG_GET_CTRL_CFG(ControllerId).pNFilterEnabled == TRUE )
      {
        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])|(CANIF_PN_FILTER_ACTIVE_MASK)));
        CanIf_CanControllerFlags[ControllerId] |= CANIF_PN_FILTER_ACTIVE_MASK;

        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
      }
#endif  /* (CANIF_PUBLIC_PN_SUPPORT == STD_ON) */
    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* this may not happen if new mode was checked as expected */
      CANIF_UNREACHABLE_CODE_ASSERT(CANIF_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  return RetVal;
}



#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
/** \brief Buffer the given message */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_BufferMessage
  (
    const PduIdType CanTxPduId,
    const Can_HwHandleType CanHthIdx,
    CONSTP2VAR( Can_PduType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* return value */
  uint8 AddMessage = 1U;            /* 1 if AddMessage() shall be called; 0 otherwise */
  CanIf_TxBufferIndexType BufIdx;   /* Tx buffer index */


  /* --- buffer the transmit message ----------------------------------- */

  /* check if L-PDU is already buffered (quite unlikely but possible) */
  BufIdx = CANIF_GET_TX_BUFFER_INDEX( CanTxPduId );
  if(    ( BufIdx != CANIF_INVALID_TX_BUFFER_INDEX )
      && ( CANIF_TX_BUFFER_IS_EMPTY( BufIdx ) != TRUE )
    )
  {
    /* buffer already used
       -> replace buffered content */
    AddMessage = 0U;

#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON )

    /* check if L-PDU is dynamic */
    if( CanTxPduId >= CanIf_ConfigPtr->nrOfStaticTxLPdus )
    {
      /* check if id changed */
      if( CanIf_TxBuffer[BufIdx].id != PduInfoPtr->id )
      {
        /* CAN Id changed
         *  -> overwrite of pending L-PDU not allowed [CANIF282], so
         *     a new buffer would be needed. there is no such buffer
         *     available as there is at most one buffer per L-PDU
         *     [CANIF071, CANIF063]:
         *     - signal "buffering not possible" by invalidating BufIdx
         */
        BufIdx = CANIF_INVALID_TX_BUFFER_INDEX;
      }
    }

#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */

  }

  /* check that a buffer was available (or already used) */
  if( BufIdx != CANIF_INVALID_TX_BUFFER_INDEX )
  {
    const CanIf_TxBufferIndexType memIndex = CanIf_TxPduConfigPtr[CanTxPduId].memIndex;

    /* write PduInfo to Tx buffer */
#if( CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON)
    CanIf_TxBuffer[BufIdx].id = PduInfoPtr->id;
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON || CANIF_METADATA_SUPPORT == STD_ON */
    CanIf_TxBuffer[BufIdx].length = PduInfoPtr->length;
    TS_MemCpy(&CanIf_TxBufferMemory[memIndex], PduInfoPtr->sdu, PduInfoPtr->length);

    /* add buffer to queue if it is not already in it */
    if( AddMessage == 1U )
    {
      CanIf_AddPendingTxRequest
        (
          CanHthIdx,
          (CanIf_LPduIndexType)CanTxPduId
        );
    }

    /* function successful */
    RetVal = E_OK;
  }
  /* else : no buffer available so return E_NOT_OK (default) */

  return RetVal;
}
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */



/** \brief Set PDU mode */
STATIC FUNC( void, CANIF_CODE ) CanIf_SetPduModeInternal
  (
    uint8 ControllerId,
    CanIf_PduModeType PduModeRequest
  )
{
  CanIf_PduModeType OldPduMode;  /* variable to store old channel mode */

#if( CANIF_SINGLE_CAN_CTRL_OPT == STD_ON )
  /* prevent ISO-C warning for overwriting parameter without using it */
  TS_PARAM_UNUSED( ControllerId );
  /* force parameter ControllerId to single controller ID #define to aid
     compiler optimization */
  ControllerId = CANIF_SINGLE_CAN_CTRL_INDEX;
#endif /* CANIF_SINGLE_CAN_CTRL_OPT == STD_ON */

  /* get network channel mode */
  OldPduMode = CanIf_PduMode[ControllerId];

  /* set new channel mode */
  switch( PduModeRequest )
  {
    case CANIF_OFFLINE:
      /* set OFFLINE mode */
      CanIf_SetOffline( ControllerId, OldPduMode);
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
      CanIf_ResetMirrBuff(ControllerId);
#endif
      break;
    case CANIF_ONLINE:
      /* set ONLINE mode */
      DBG_CANIF_PDUMODE_GRP(ControllerId,(CanIf_PduMode[ControllerId]),(CANIF_ONLINE));
      CanIf_PduMode[ControllerId] = CANIF_ONLINE;
      break;

#if( CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
    case CANIF_TX_OFFLINE_ACTIVE:
      /* enable TX_OFFLINE_ACTIVE mode */
      CanIf_SetTxOfflineActive(ControllerId, OldPduMode);
      break;
#endif
    /* CANIF_TX_OFFLINE */
    default:
      /* SWS_CANIF_00489: disable Tx path, clear the corresponding CanIf buffers, enable Rx path */
      DBG_CANIF_PDUMODE_GRP(ControllerId,(CanIf_PduMode[ControllerId]),(CANIF_TX_OFFLINE));
      CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE;
#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
    /* no transmissions allowed -> clear Tx message buffers */
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    CanIf_FreeControllerTxBuffers( ControllerId, FALSE );
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    CanIf_FreeControllerTxBuffers( ControllerId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
      CanIf_ResetMirrBuff(ControllerId);
#endif

#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
      /* SWS_CANIF_00878: Enable partial networking filter when PduModeRequest is set to CANIF_TX_OFFLINE */
      if(CANIF_CFG_GET_CTRL_CFG(ControllerId).pNFilterEnabled == TRUE )
      {
         SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

         DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])|(CANIF_PN_FILTER_ACTIVE_MASK)));
         CanIf_CanControllerFlags[ControllerId] |= CANIF_PN_FILTER_ACTIVE_MASK;

         SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
      }
#endif  /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */

      break;
  }
}



#if(CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON)
STATIC FUNC( void, CANIF_CODE ) CanIf_SetTxOfflineActive
  (
    uint8 ControllerId,
    CanIf_PduModeType OldPduMode
  )
{
  /* disable Tx path */
  switch( OldPduMode )
  {
      case CANIF_ONLINE:
        DBG_CANIF_PDUMODE_GRP(ControllerId,(CanIf_PduMode[ControllerId]),(CANIF_TX_OFFLINE_ACTIVE));
        CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE_ACTIVE;
#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
    /* clear Tx message buffers */
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    CanIf_FreeControllerTxBuffers( ControllerId, FALSE );
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    CanIf_FreeControllerTxBuffers( ControllerId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
#endif /* (CANIF_PUBLIC_TX_BUFFERING == STD_ON) */
      break;

      case CANIF_TX_OFFLINE:
      case CANIF_OFFLINE:
        DBG_CANIF_PDUMODE_GRP(ControllerId,(CanIf_PduMode[ControllerId]),(CANIF_TX_OFFLINE_ACTIVE));
        CanIf_PduMode[ControllerId] = CANIF_TX_OFFLINE_ACTIVE;
      break;

      default:
       /* Tx path already disabled - nothing to do */
      break;
  }
}
#endif /* (CANIF_TX_OFFLINE_ACTIVE_SUPPORT == STD_ON) */

STATIC FUNC( void, CANIF_CODE ) CanIf_SetOffline
  (
    uint8 ControllerId,
    CanIf_PduModeType OldPduMode
  )
{
  /* set OFFLINE mode */
  CanIf_PduMode[ControllerId] = CANIF_OFFLINE;
#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )
  if( OldPduMode == CANIF_ONLINE )
  {
    /* transmit path was disabled (and previously enabled) */
    /* clearing old pending Tx requests */
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    CanIf_FreeControllerTxBuffers( ControllerId, FALSE );
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    CanIf_FreeControllerTxBuffers( ControllerId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
  }
#else /* CANIF_PUBLIC_TX_BUFFERING == STD_OFF */
  TS_PARAM_UNUSED(OldPduMode);
#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */

}

#if( CANIF_READRXPDU_DATA_API == STD_ON )
/** \brief Write Rx buffer */
STATIC FUNC( void, CANIF_CODE ) CanIf_WriteRxBuffer
  (
    CONSTP2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST )
      RxLPduConfig,
    CONSTP2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  )
{
  CanIf_RxBufferIndexType RxBufStartIdx = RxLPduConfig->rxBufferIndex; /* Rx buffer start index */

  if( RxBufStartIdx != CANIF_INVALID_RX_BUFFER_INDEX )
  {
    uint8 i;            /* loop variable */

    /* write DLC - CanIf Pdus are at maximum 8 bytes long */
    CanIf_RxBuffer[RxBufStartIdx] = (uint8)PduInfoPtr->SduLength;
    /* increment RxBufStartIdx to get first position after DLC */
    RxBufStartIdx++;
    /* write data */
    for( i = 0U; i < PduInfoPtr->SduLength; i++ )
    {
      CanIf_RxBuffer[RxBufStartIdx+i] = PduInfoPtr->SduDataPtr[i];
    }
  }

}
#endif /* ( CANIF_READRXPDU_DATA_API == STD_ON ) */


#if (CANIF_SW_FILTER == STD_ON)
/** \brief CAN interface software filtering */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SoftwareFiltering
  (
    const uint16 CanIfHrhIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  )
{
  Std_ReturnType RetVal = E_NOT_OK; /* function return value */
  CanIf_LPduIndexType FirstIdx;           /* first nw. index */
  CanIf_LPduIndexType LastIdx;            /* last nw. index */

  /* initialize variables */
  FirstIdx = CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx).firstIndex;
  LastIdx = CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx).lastIndex;

  /* check if HRH is used at all */
#if (CANIF_METADATA_SUPPORT == STD_OFF)
  if((LastIdx != CANIF_INVALID_LPDU_INDEX)
    &&(FirstIdx != CANIF_INVALID_LPDU_INDEX))
#else
  if(LastIdx != CANIF_INVALID_LPDU_INDEX)
#endif  /* CANIF_METADATA_SUPPORT == STD_OFF */
  {
    /* check if software filtering is allowed */
    if( CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx).swFiltering ==
        CANIF_BINARY_SOFTWARE_FILTERING
      )
    {
      /* perform the software filtering */
#if (CANIF_METADATA_SUPPORT == STD_ON)
      RetVal = CanIf_PerformSwFiltering(CanIfHrhIdx, FirstIdx, LastIdx, CanId, Index);
#else
      RetVal = CanIf_PerformSwFiltering(FirstIdx, LastIdx, CanId, Index);
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */

    }
    else
    {
      /* no software filtering is allowed -> take the first element */
#if (CANIF_METADATA_SUPPORT == STD_ON)
      if(  FirstIdx != CANIF_INVALID_LPDU_INDEX )
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
      {
        *Index = FirstIdx;
      }
#if (CANIF_METADATA_SUPPORT == STD_ON)
      else
      {
        *Index = LastIdx;
      }
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
      RetVal = E_OK;
    }
  }
  else
  {
    /* no software filtering possible since no L-PDU assigned to HRH */
    RetVal = E_NOT_OK;
  }
  return RetVal;
}
#endif /* CANIF_SW_FILTER == STD_ON */

#if (CANIF_SW_FILTER == STD_ON)
/** \brief Perform software filtering */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_PerformSwFiltering
  (
#if (CANIF_METADATA_SUPPORT == STD_ON)
    const uint16 CanIfHrhIdx,
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
    const CanIf_LPduIndexType FirstIdx,
    CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  )
{
  Std_ReturnType      RetVal = E_NOT_OK;  /* function return value */
#if (CANIF_METADATA_SUPPORT == STD_ON)
  const CanIf_LPduIndexType LastMaskedIdx = CANIF_CFG_GET_HRH_CFG(CanIfHrhIdx).lastMaskedIndex;
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */

#if (CANIF_METADATA_SUPPORT == STD_ON)
  if(FirstIdx != CANIF_INVALID_LPDU_INDEX)
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
  {
    if(FirstIdx == LastIdx)
    {
      /* single Rx L-PDU assigned */
      RetVal = CANIF_SINGLE_PDU_SW_FILTERING( FirstIdx, CanId );
      if( RetVal == E_OK )
      {
        *Index = FirstIdx;
      }
    }
    else
    {
      /* several Rx L-PDUs assigned */
      RetVal = CanIf_BinarySwFiltering(FirstIdx, LastIdx, CanId, Index);
    }
  }
#if (CANIF_METADATA_SUPPORT == STD_ON)
  if((E_NOT_OK == RetVal) && (LastMaskedIdx != CANIF_INVALID_LPDU_INDEX))
  {
    if(FirstIdx != CANIF_INVALID_LPDU_INDEX)
    {
      LastIdx = LastIdx+1U; /* start from the index of the masked CanIds*/
    }
    RetVal = CanIf_LinearSwFiltering(LastIdx, LastMaskedIdx, CanId, Index);
  }
#endif  /* CANIF_METADATA_SUPPORT == STD_ON */
  return RetVal;
}
#endif /* CANIF_SW_FILTER == STD_ON */

#if (CANIF_SW_FILTER == STD_ON)
/** \brief Perform binary software filtering */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_BinarySwFiltering
  (
    const CanIf_LPduIndexType FirstIdx,
    const CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  )
{
  /* Annotations:
   * - The function uses sint32 variables because in this way it can be
   *   executed faster on the current 32 bit targets than with sint16
   *   values. The performance will however suffer from future 16 bit
   *   targets.
   * - Signed variables are necessary because otherwise an underflow for
   *   "middle" during the calculation may not be detected resulting in
   *   erroneous behaviour (middle == 0 and CAN ID lower than the smallest
   *   configured CAN ID for this HRH).
   * - Before comparison the IDs are explicitly cast to uint32 to make
   *   sure that the order from configuration is correctly used by this
   *   C code.
   */

  Std_ReturnType RetVal = E_NOT_OK;
                                    /* functions return value */
  sint32 left = (sint32)FirstIdx;
                                    /* left index border */
  sint32 right = (sint32)LastIdx;
                                    /* right index border */
  sint32 middle;                    /* temp. index variable */

  /* CHECK: NOPARSE */
  if( left > right )
  {
    /* No PDU, default return value is correct */
    CANIF_UNREACHABLE_CODE_ASSERT(CANIF_INTERNAL_API_ID);
  }
  /* search a direct Rx L-PDU hit */
  else
  /* CHECK: PARSE */
  {
    uint32 cfgCanId;

    do
    {
      /* sint32 is large enough to not produce any overflows */
      middle = ((left + right) / (sint32) 2);
      cfgCanId = (uint32) CANIF_CFG_GET_SORTEDRXLPDU_CFG(middle).canId;

      if( cfgCanId == (uint32)CanId )
      {
        /* ID found */
        *Index = (CanIf_LPduIndexType)middle;

        /* stop loop */
        RetVal = E_OK;
        break;
      }
      else
      {
        if( (uint32)CanId < cfgCanId)
        {
          right = (middle - (sint32) 1);
        }
        else
        {
          left = (middle + (sint32) 1);
        }
      }
    } while( left <= right );

#if( CANIF_RANGE_RECEPTION == STD_ON )
    /* look for a range configuration hit */
    if( RetVal == E_NOT_OK )
    {
      /* The element with index "middle" now has either
         - the smallest CAN ID greater than the searched one or
         - the largest CAN ID smaller than the searched one
         out of the PDUs configured for this HRH due to the behaviour of the
         binary search. */

      /* if CAN ID of current PDU is greater than the searched one
         => take the previous PDU */
      if( cfgCanId > (uint32)CanId )
      {
        middle--;
      }
      /* now check if the PDU referenced by "middle" has a range which covers
         the searched ID; check of "middle" is necessary because of possible
         previous adaptation */
      if(    ( middle >= (sint32)FirstIdx )
          && (    ( CANIF_GET_UPPER_RANGE_CANID( middle ) != CANIF_NO_RANGE_CONFIG )
               && ( CANIF_GET_UPPER_RANGE_CANID( middle ) >= (uint32)CanId )
             )
        )
      {
        /* ID found */
        *Index = (CanIf_LPduIndexType)middle;

        /* set return value */
        RetVal = E_OK;
      }
      /* else : No PDU found */
    }
#endif /* CANIF_RANGE_RECEPTION == STD_ON */
  }

  return RetVal;
}
#endif /* CANIF_SW_FILTER == STD_ON */

#if ((CANIF_METADATA_SUPPORT == STD_ON) && (CANIF_SW_FILTER == STD_ON))
/** \brief Perform linear software filtering */
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_LinearSwFiltering
  (
    const CanIf_LPduIndexType FirstIdx,
    const CanIf_LPduIndexType LastIdx,
    const Can_IdType CanId,
    CONSTP2VAR( CanIf_LPduIndexType, AUTOMATIC, CANIF_APPL_DATA ) Index
  )
{
  uint32 indexIterator = FirstIdx;
  uint32 cfgCanId;
  Std_ReturnType RetVal = E_NOT_OK; /* functions return value */

  for(indexIterator=FirstIdx;indexIterator<=LastIdx;indexIterator++)
  {
    cfgCanId = (uint32) CANIF_CFG_GET_SORTEDRXLPDU_CFG(indexIterator).canId;
    /* !LINKSTO CanIf.EB.Metadata.Functional.2, 1 */
    if( cfgCanId == (((uint32)CanId) & CANIF_CFG_GET_SORTEDRXLPDU_CFG(indexIterator).CanIfRxPduCanIdMask))
    {
      /* ID found */
      *Index = (CanIf_LPduIndexType)indexIterator;

      /* stop loop */
      RetVal = E_OK;
      break;
    }
  }
  return RetVal;
}
#endif  /* (CANIF_METADATA_SUPPORT == STD_ON) && (CANIF_SW_FILTER == STD_ON) */

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_RxIndication_DetChecks
  (
    Can_HwHandleType Hrh,
    Can_IdType CanId,
    PduLengthType CanDlc,
    P2CONST( uint8, AUTOMATIC, CANIF_APPL_DATA ) CanSduPtr
  )
  {
  Std_ReturnType RetCode = E_NOT_OK;

  /* parameter Hrh */
#if( CANIF_HOH_TRANSLATION_OPT == STD_ON )
  if( Hrh >= CanIf_DriverConfigPtr->nrOfHrhs )
#else
    if( Hrh >= CanIf_DriverConfigPtr->upperHohIdLimit )
#endif /* CANIF_HOH_TRANSLATION_OPT == STD_ON */
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
          CANIF_INSTANCE_ID,
          CANIF_SID_RXINDICATION,
          CANIF_E_PARAM_HRH
      );
    }

  /* parameter CanId (dependent on CAN ID type) */
    else if( CANIF_CHECK_FOR_INVALID_CAN_ID( CanId ) )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
          CANIF_INSTANCE_ID,
          CANIF_SID_RXINDICATION,
          CANIF_E_PARAM_CANID
      );
    }

  /* parameter DLC */
    else if( CanDlc > CANIF_MAXIMUM_CAN_FD_DLC )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
          CANIF_INSTANCE_ID,
          CANIF_SID_RXINDICATION,
          CANIF_E_PARAM_DLC
      );
    }

  /* parameter CanSduPtr */
    else if( CanSduPtr == NULL_PTR )
    {
      /* report DET error */
      CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
          CANIF_INSTANCE_ID,
          CANIF_SID_RXINDICATION,
          CANIF_E_PARAM_POINTER
      );

    }
    else
    {
#if (CANIF_HOH_TRANSLATION_OPT == STD_OFF)
      const uint16 CanIfHrhIdx = CANIF_GET_HRH_INDEX( Hrh );

      if(    (CanIfHrhIdx == CANIF_INVALID_HOH_ID)
          || ((CanIfHrhIdx & CANIF_HTH_ID_MASK) != 0U)
      )
      {
        /* report DET error */
        CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
            CANIF_INSTANCE_ID,
            CANIF_SID_RXINDICATION,
            CANIF_E_PARAM_HRH
        );
      }
      else
#endif /* (CANIF_HOH_TRANSLATION_OPT == STD_OFF) */
      {
        RetCode = E_OK;
      }
    }

  return RetCode;
}

#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */




#if( CANIF_DLC_CHECK_ENABLED == STD_ON )

STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_DlcHandling
  (
    PduLengthType CanDlc,
    P2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST ) RxLPduConfigPtr,
    P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr
  )
{
  Std_ReturnType RetCode = E_NOT_OK;

  /* --- DLC check ------------------------------------------------------- */
  if( CanDlc < RxLPduConfigPtr->dlc )
  {
    /* reception not allowed -> abort */

    CANIF_DLC_ERROR_NOTIFICATION( RxLPduConfigPtr );

#if( CANIF_DEV_ERROR_DETECT == STD_ON )
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_RXINDICATION,
                            CANIF_E_INVALID_DLC
                          );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON*/

  }
  else
  {

    /* reception is allowed -> continue */

    CANIF_DLC_PASSED_NOTIFICATION( RxLPduConfigPtr );

    /* CanIf shall pass the received(!) DLC in both cases:
       a) if the DLC check is disabled for this L-PDU
       b) if the DLC check is passed */
    PduInfoPtr->SduLength = CanDlc;

    RetCode = E_OK;

  }

  return RetCode;
}
#endif /* CANIF_DLC_CHECK_ENABLED == STD_ON */


#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
/** \brief Clear controller Tx confirmation Flag */
STATIC FUNC( void, CANIF_CODE ) CanIf_ClearTxConfirmationFlag
  (
    const uint8 ControllerId
  )
{
    /* enter critical section */
    SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

    if( (CanIf_CanControllerFlags[ControllerId] & CANIF_TX_CONFIRMATION_MASK) != 0U )
    {
      uint8 value = (CanIf_CanControllerFlags[ControllerId])&((uint8)(~CANIF_TX_CONFIRMATION_MASK));
      DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]), value);
      CanIf_CanControllerFlags[ControllerId] = value;
    }

    /* leave critical section */
    SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
}
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

#if( CANIF_PUBLIC_TX_BUFFERING == STD_ON )

/** \brief Free controller transmit buffers */
STATIC FUNC( void, CANIF_CODE ) CanIf_FreeControllerTxBuffers
  (
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    const uint8 ControllerId,
    boolean callTxConf
#else  /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    const uint8 ControllerId
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */

  )
{
  Can_HwHandleType i;     /* loop variable */
  CanIf_LPduIndexType j;  /* loop variable */
  PduIdType TxPduId;      /* Tx L-PDU ID */
  P2CONST( CanIf_HthConfigType, AUTOMATIC, CANIF_APPL_CONST )
    HthConfigPtr = &CANIF_CFG_GET_HTH_CFG(0U); /* HTH array */

  /* enter critical section */
  SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  /* for each HTH ... */
  for( i = 0U; i < CanIf_DriverConfigPtr->nrOfHths; i++ )
  {
    /* ... check if this HTH belongs to the given controller */
    if( CANIF_GET_HOH_INT_CTRL_IDX(HthConfigPtr[i]) == ControllerId )
    {
      /* HTH belongs to the given controller */
      if( CanIf_NrOfPendingTxRequests[i] > 0U )
      {
        /* There are pending transmit requests */

        /* for all pending Tx requests ... */
        for( j = (HthConfigPtr[i]).firstIndex +
                 CanIf_NrOfPendingTxRequests[i];
             j > (HthConfigPtr[i]).firstIndex;
             j-- )
        {
          CanIf_TxBufferIndexType BufIdx; /* Tx buffer index */

          /* j is used as "index+1" because it might be an unsigned
             variable and otherwise an underflow could happen */
          TxPduId = CanIf_PendingTxBuffers[j-1U];
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
          if(callTxConf)
          {
              /* Send Negative Tx Confirmation for pending buffered Tx Pdus */
              /* call Tx confirmation callback with E_NOT_OK */
              /* !LINKSTO CanIf.EB.User_TxConfirmation.Signature.Result.ON, 1 */
              CanIf_SendTxConfirmation( TxPduId, E_NOT_OK);
          }
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
          BufIdx = CANIF_GET_TX_BUFFER_INDEX( TxPduId );
          /* CHECK: NOPARSE */
          if( BufIdx == CANIF_INVALID_TX_BUFFER_INDEX )
          {
              CANIF_UNREACHABLE_CODE_ASSERT(CANIF_INTERNAL_API_ID);
          }
          else
          /* CHECK: PARSE */
          {
            /* ... release Tx buffers */
            CANIF_INVALIDATE_TX_BUFFER( BufIdx );
          }
        }

        /* correct number of pending Tx requests */
        CanIf_NrOfPendingTxRequests[i] = 0U;
      }
      /* else : no pending Tx requests */
    }
    /* else : HTH doesn't belong to searched controller */
  }

  /* leave critical section */
  SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

}



/** \brief Add new Tx request to pending list */
STATIC FUNC( void, CANIF_CODE ) CanIf_AddPendingTxRequest
  (
    const Can_HwHandleType CanHthIdx,
    const CanIf_LPduIndexType CanTxPduId
  )
{
  uint32 NewCanId;  /* CAN ID of new msg. */
  uint32 CmpCanId;  /* CAN ID for comparisson */

  CanIf_LPduIndexType CurIdx;   /* current index */
  CanIf_LPduIndexType FirstIdx; /* first index for hth */

  FirstIdx = CANIF_CFG_GET_HTH_CFG(CanHthIdx).firstIndex;
  CurIdx = FirstIdx + CanIf_NrOfPendingTxRequests[CanHthIdx];

  /* --- add new message to list --------------------------------------- */

  /* sort list with new element */
  /* NOTE
     A kind of insertion sort is used because in each case all messages
     with higher priority must be moved. Therefore an additional comparison
     is not too much additional overhead especially if the number of
     concurrently stored L-PDUs is low. */
  if( CurIdx > FirstIdx )
  {
    NewCanId = (uint32)CANIF_GET_BUFFERED_CAN_ID( CanTxPduId );
    NewCanId = CANIF_CONVERT_ID_FOR_COMPARISON( NewCanId );

    CmpCanId = (uint32)CANIF_GET_BUFFERED_CAN_ID( CanIf_PendingTxBuffers[CurIdx-1U] );
    CmpCanId = CANIF_CONVERT_ID_FOR_COMPARISON( CmpCanId );

    while( CmpCanId < NewCanId )
    {
      /* "old" message has higher priority -> move it */
      CanIf_PendingTxBuffers[CurIdx] = CanIf_PendingTxBuffers[CurIdx-1U];
      CurIdx--;

      /* get CAN ID of next message */
      if( CurIdx > FirstIdx )
      {
        CmpCanId = (uint32)CANIF_GET_BUFFERED_CAN_ID( CanIf_PendingTxBuffers[CurIdx-1U] );
        CmpCanId = CANIF_CONVERT_ID_FOR_COMPARISON( CmpCanId );
      }
      else
      {
        /* abort loop */
        CmpCanId = NewCanId;
      }
    }
  }

  /* write Tx L-PDU ID and increase number of pending messages */
  CanIf_PendingTxBuffers[CurIdx] = CanTxPduId;
  (CanIf_NrOfPendingTxRequests[CanHthIdx])++;

}



/** \brief Send next Tx request */
STATIC FUNC( void, CANIF_CODE ) CanIf_SendNextTxRequest
  ( const Can_HwHandleType CanHth, const uint8 ControllerId )
{

  CanIf_TxBufferIndexType PendNr;         /* number of pending requests */
  Can_ReturnType CanRetVal = CAN_NOT_OK;  /* Can driver return value */

  /* Internal HTH index */
  const Can_HwHandleType CanHthIdx = (Can_HwHandleType) CANIF_GET_HTH_INDEX(CanHth + CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].HohIdOffset);
  /* first pending request index */
  const CanIf_LPduIndexType StartIdx = CANIF_CFG_GET_HTH_CFG(CanHthIdx).firstIndex;

#if( CANIF_MULTIPLE_DRV_SUPPORT == STD_OFF )
  TS_PARAM_UNUSED( ControllerId );
#endif /* CANIF_MULTIPLE_DRV_SUPPORT == STD_OFF */

  /* check if there is a pending transmission (and that no previous CAN
     transmission was performed successful - in case that an erroneous
     message was buffered this message must be removed from the queue) */
  /* NOTE that the caller of this function must have made sure that a message
     transmission is allowed at all by checking controller and PDU mode and
     by making the call in a critical section that cannot be interrupted! */
  do
  {
    /* flag if transmission shall be done */
    boolean doTransmit = FALSE;
    /* index of Tx buffer */
    CanIf_TxBufferIndexType BufferIdx = CANIF_INVALID_TX_BUFFER_INDEX;
    /* CAN PDU */
    Can_PduType CanPduInfo;
    CanIf_LPduIndexType CanTxPduId = CANIF_INVALID_LPDU_INDEX;

    /* enter critical section */
    SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

    /* get number of pending transmissions */
    PendNr = CanIf_NrOfPendingTxRequests[CanHthIdx];

    if( PendNr > 0U )
    {
      /* get pending Tx L-PDU ID and associated buffer index*/
      CanTxPduId = CanIf_PendingTxBuffers[(StartIdx+PendNr)-1U];

      BufferIdx = CANIF_GET_TX_BUFFER_INDEX( CanTxPduId );

      /* CHECK: NOPARSE */
      if(    ( BufferIdx == CANIF_INVALID_TX_BUFFER_INDEX )
          || ( CANIF_TX_BUFFER_IS_EMPTY( BufferIdx ) == TRUE )
        )
      {
        /* there is no Tx buffer assigned to this L-PDU or the buffer is empty
           -> internal ERROR
           -> remove L-PDU from the list */
        (CanIf_NrOfPendingTxRequests[CanHthIdx])--;
        CANIF_UNREACHABLE_CODE_ASSERT(CANIF_INTERNAL_API_ID);
      }
      else
      /* CHECK: PARSE */
      {
        const CanIf_TxBufferIndexType memIndex = CanIf_TxPduConfigPtr[CanTxPduId].memIndex;

        /* copy PDU information */
        CanPduInfo.id = CANIF_GET_BUFFERED_CAN_ID( CanTxPduId );
        CanPduInfo.length = (CanIf_TxBuffer[BufferIdx]).length;
        CanPduInfo.sdu = &CanIf_TxBufferMemory[memIndex];
        CanPduInfo.swPduHandle = CanTxPduId;

        /* we have a pending message and a valid buffer*/
        doTransmit = TRUE;
      }
    }

    /* leave critical section */
    SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

    if ( doTransmit )
    {
      /* Call Can_Write() and transmit the Tx L-PDU */
#if (!defined CANIF_CAN_ASR431_COMPATIBILITY) && (!defined CANIF_CAN_ASR440_COMPATIBILITY)
      CanRetVal = CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(CANIF_CFG_GET_HTH_CFG(CanHthIdx).intCtrlIdx)].Write( CanHth, &CanPduInfo );
#else
      /* cast to uint8 is done because casting an enum type to another is raising warnings, even if the two types are compatible */
      CanRetVal = (Can_ReturnType)(uint8)CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(CANIF_CFG_GET_HTH_CFG(CanHthIdx).intCtrlIdx)].Write( CanHth, &CanPduInfo );
#endif

      /* check result of transmission */
      if( CanRetVal != CAN_BUSY )
      {
        /* enter critical section */
        SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

        /* either message was successfully transmitted or message is erroneous
           -> remove message from buffer */
        CANIF_INVALIDATE_TX_BUFFER( BufferIdx );

        /* check if a bus-off occured (see ASCCANIF-1286) */
        if(0U < CanIf_NrOfPendingTxRequests[CanHthIdx])
        {
          /* decrease number of pending Tx requests */
          (CanIf_NrOfPendingTxRequests[CanHthIdx])--;
        }

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
        /* Block delimited for Bus Mirroring */
        {
          const uint8 controllerId = CANIF_GET_HOH_INT_CTRL_IDX(CANIF_CFG_GET_HTH_CFG(CANIF_GET_HTH_INDEX(CanHth)));
          if(CANIF_BUS_MIRRORING_MASK == (CanIf_CanControllerFlags[controllerId] & CANIF_BUS_MIRRORING_MASK))
          {
            TS_MemCpy(&CanIf_MirrorBuffPayload[CanIf_TxPduConfigPtr[CanTxPduId].mirrBuffOfs], CanPduInfo.sdu, CanPduInfo.length);
            CanIf_MirrorBuff[CanTxPduId].status = CANIF_FRAME_READY_TO_MIRROR;
            CanIf_MirrorBuff[CanTxPduId].length = CanPduInfo.length;
          }
        } /* end mirroring block */
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

        /* leave critical section */
        SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
      }
    }
  } while( ( PendNr > 0U ) && ( CanRetVal == CAN_NOT_OK ) );

}



/** \brief Clear Tx requests of given HTH */
STATIC FUNC( void, CANIF_CODE ) CanIf_ClearHthTxRequests
  ( const Can_HwHandleType CanHth, const uint8 ControllerId )
{
  Can_HwHandleType CanHthIdx; /* Internal HTH index */
  PduIdType CanTxPduId;       /* CAN Tx L-PDU ID */
  CanIf_LPduIndexType StartIdx;
                                            /* first pending request index */
  CanIf_LPduIndexType PendNr;
                                            /* number of pending requests */
#if( CANIF_MULTIPLE_DRV_SUPPORT == STD_OFF )
  TS_PARAM_UNUSED( ControllerId );
#endif /* CANIF_MULTIPLE_DRV_SUPPORT == STD_OFF */

  /* enter critical section */
  SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  /* initialize variables */
  CanHthIdx = (Can_HwHandleType) CANIF_GET_HTH_INDEX(CanHth + CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].HohIdOffset);

  /* read start index and number of pending requests */
  StartIdx = CANIF_CFG_GET_HTH_CFG(CanHthIdx).firstIndex;
  PendNr = CanIf_NrOfPendingTxRequests[CanHthIdx];

  while( PendNr > 0U )
  {
    /* there is at least one message pending for transmission */

    CanIf_TxBufferIndexType BufferIdx;  /* index of Tx buffer */

    /* get pending Tx L-PDU ID and associated buffer index */
    CanTxPduId = CanIf_PendingTxBuffers[(StartIdx+PendNr)-1U];
    BufferIdx = CANIF_GET_TX_BUFFER_INDEX( CanTxPduId );

    /* release buffer if it is used */
    /* CHECK: NOPARSE */
    if(    ( BufferIdx == CANIF_INVALID_TX_BUFFER_INDEX )
        || ( CANIF_TX_BUFFER_IS_EMPTY( BufferIdx ) == TRUE )
      )
    {
      CANIF_UNREACHABLE_CODE_ASSERT(CANIF_INTERNAL_API_ID);
    }
    else
    /* CHECK: PARSE */
    {
      CANIF_INVALIDATE_TX_BUFFER( BufferIdx );
    }

    /* decrease number of pending Tx requests (either a buffer was released
       or the value was at least one too high and is implicitly corrected) */
    (CanIf_NrOfPendingTxRequests[CanHthIdx])--;

    /* read current number of pending requests */
    PendNr = CanIf_NrOfPendingTxRequests[CanHthIdx];
  }

  /* leave critical section */
  SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

}

#endif /* CANIF_PUBLIC_TX_BUFFERING == STD_ON */


#if( (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) )
STATIC FUNC( void, CANIF_CODE ) CanIf_SendRxIndication
  (
    uint8 userType,
    PduIdType RxLPduId,
    P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr,
    Can_IdType CanId
  )
{

  if( userType != CANIF_NO_UPPER_LAYER_CALLBACK )
  {
#if( CANIF_UL_RX_INDICATION_SUPPORT == STD_ON )
    if( CanIf_CbkFctPtrTbl[userType].UlRxIndFctPtr != NULL_PTR )
    {
      (CanIf_CbkFctPtrTbl[userType].UlRxIndFctPtr)( RxLPduId, PduInfoPtr );
    }
    else
#endif /* CANIF_UL_RX_INDICATION_SUPPORT == STD_ON */
    {
#if( CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON )
      if( CanIf_CbkFctPtrTbl[userType].CddRxIndFctPtr != NULL_PTR )
      {
        /* VAG_CANIF_093: add parameter CanId;
           for the upper layer it is important to use the real CAN ID and not
           the internally used configured one */
        (CanIf_CbkFctPtrTbl[userType].CddRxIndFctPtr)( RxLPduId, PduInfoPtr, CanId );
      }
      /* else: No RxIndication function configured for this Rx-LPdu */
#else /* CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON */
      TS_PARAM_UNUSED( CanId );
#endif /* CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON */
    }
  }
  /* else: No upper layer configured for this Rx-LPdu */

}
#endif /* (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) */


#if( CANIF_TX_CONFIRMATION_SUPPORT == STD_ON )
STATIC FUNC( void, CANIF_CODE ) CanIf_SendTxConfirmation
  (
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    PduIdType CanTxPduId,
    Std_ReturnType result
#else /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON  */
    PduIdType CanTxPduId
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
  )
{
  uint8 userType = CanIf_TxPduConfigPtr[CanTxPduId].userType;
  PduIdType sourcePduId = CanIf_TxPduConfigPtr[CanTxPduId].sourcePduId_UL;

  if( userType != CANIF_NO_UPPER_LAYER_CALLBACK )
  {
    if( CanIf_CbkFctPtrTbl[userType].TxConfFctPtr != NULL_PTR )
    {
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
      if(result == E_OK)
      {
        (CanIf_CbkFctPtrTbl[userType].TxConfFctPtr)( sourcePduId );
      }
      else
      {
        /* Nothing to do, as we call the callback without result parameter in case of E_OK only*/
      }
#else
      (CanIf_CbkFctPtrTbl[userType].TxConfFctPtr)( sourcePduId );
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    }
#if( CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON )
    else if(CanIf_CbkFctPtrTbl[userType].TxConfFctResultPtr != NULL_PTR)
    {
      (CanIf_CbkFctPtrTbl[userType].TxConfFctResultPtr)( sourcePduId, result);
    }
#endif /* CANIF_PUBLIC_TXCONFIRM_RESULT_SUPPORTED == STD_ON */
    else
    {
      /* else: No TxConfirmation function configured for this Tx-LPdu */
    }
  }
  /* else: No upper layer configured for this Tx-LPdu */

}

#if( CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON )

STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_InsertTxQueue
  (
    PduIdType TxPduId
  )
{
  Std_ReturnType RetVal = E_NOT_OK;
  const uint8 MfIndex = CanIf_TxPduConfigPtr[TxPduId].MFIndex;
  const CanIf_MfTxPtrTblType MfTbl = CanIf_MfTxFctPtrTbl[MfIndex];

  SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  if(CanIf_TotalTxQueueSize[MfIndex] < MfTbl.TxQueueSize)
  {
    CanIf_TxPduQueue[CanIf_TotalTxQueueSize[MfIndex] + MfTbl.TxQueueOffset].MFIdx = MfIndex;
    CanIf_TxPduQueue[CanIf_TotalTxQueueSize[MfIndex] + MfTbl.TxQueueOffset].PduId = TxPduId;

    CanIf_TotalTxQueueSize[MfIndex]++;

    RetVal= E_OK;
  }

  SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  return RetVal;

}

STATIC FUNC(void, CANIF_CODE) CanIf_InitTxQueue
  (
    void
  )
{
  uint16 i;

  for(i = 0U; i < CANIF_MAX_QUEUED_TXPDUS_SIZE; i++)
  {
    CanIf_TxPduQueue[i].MFIdx = CANIF_INVALID_MF_INDEX;
  }

  for(i = 0U; i< CANIF_TX_MF_SIZE; i++)
  {
    CanIf_TotalTxQueueSize[i] = 0U;

#if(CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    CanIf_NoOfTxPdusExceedingQueue[i] = 0U;
#endif /* CANIF_TX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
  }
}

#endif /* CANIF_DECOUPLED_TXPROCESSING_SUPPORT == STD_ON */

#endif /* CANIF_TX_CONFIRMATION_SUPPORT == STD_ON */


#if( CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON )

STATIC FUNC( Std_ReturnType, CANIF_CODE ) CanIf_InsertRxQueue
  (
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_CONST ) PduInfoPtr,
    Can_IdType CanId,
    CanIf_LPduIndexType RxPduId,
    uint8 ControllerId
  )
{
  CONSTP2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST ) RxLPduConfig =
      &(CanIf_RxPduConfigPtr[RxPduId]);

  Std_ReturnType RetVal = E_NOT_OK;
  const uint8 MfIndex = RxLPduConfig->MFIndex;
  const CanIf_MfRxPtrTblType MfTbl = CanIf_MfRxFctPtrTbl[MfIndex];
  const uint16 MfPayloadMaxVal = (uint16)MfTbl.PayloadQueueOffset + MfTbl.PayloadQueueSize;
  uint16 MfCurrentIdx;

  SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  MfCurrentIdx = CanIf_MFRxQueueIdx[MfIndex] + (uint16)PduInfoPtr->SduLength + 1U;

  if((CanIf_TotalRxQueueSize[MfIndex] < MfTbl.RxQueueSize) &&
     (MfCurrentIdx < MfPayloadMaxVal))
  {
    const uint16 PduIdx = CanIf_TotalRxQueueSize[MfIndex] + MfTbl.RxQueueOffset;
    CONSTP2VAR( CanIf_RxQueueType, AUTOMATIC, CANIF_APPL_DATA) RxPduQueue = &CanIf_RxPduQueue[PduIdx];
    uint16 QueueDataPos = CanIf_MFRxQueueIdx[MfIndex];
    uint8 idx;

    RxPduQueue->PreviousQueuePos = QueueDataPos;
    RxPduQueue->PayloadLen = (uint8)PduInfoPtr->SduLength;

    /* write data */
    for(idx = 0U; idx < PduInfoPtr->SduLength; idx++)
    {
      CanIf_RxDataQueue[QueueDataPos + idx] = PduInfoPtr->SduDataPtr[idx];
    }

    RxPduQueue->CanId = CanId;
    RxPduQueue->PduId = RxPduId;
    RxPduQueue->ControllerId = ControllerId;
    RxPduQueue->MFIdx = MfIndex;

    CanIf_MFRxQueueIdx[MfIndex] = QueueDataPos + idx;
    CanIf_TotalRxQueueSize[RxLPduConfig->MFIndex]++;

    RetVal= E_OK;
  }

  SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

  return RetVal;
}

STATIC FUNC(void, CANIF_CODE) CanIf_InitRxQueue
  (
    void
  )
{
  uint16 i;

  for(i = 0U; i < CANIF_MAX_QUEUED_RXPDUS_SIZE; i++)
  {
    CanIf_RxPduQueue[i].MFIdx = CANIF_INVALID_MF_INDEX;
  }

  for(i = 0U; i < CANIF_RX_MF_SIZE; i++)
  {
    CanIf_TotalRxQueueSize[i] = 0U;
    CanIf_MFRxQueueIdx[i] = CanIf_MfRxFctPtrTbl[i].PayloadQueueOffset;

#if(CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON)
    CanIf_NoOfRxPdusExceedingQueue[i] = 0U;
#endif /* CANIF_RX_DECOUPLED_MEASUREMENT_SUPPORT == STD_ON */
  }
}

#endif /* CANIF_DECOUPLED_RXPROCESSING_SUPPORT == STD_ON */


/** \brief Process the received Rx Pdu */
STATIC FUNC(void, CANIF_CODE) CanIf_ProcessRxPdu
(
  P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr,
  Can_IdType CanId,
  CanIf_LPduIndexType RxPduId,
  uint8 ControllerId
)
{
  /* set Rx L-PDU config pointer */
  CONSTP2CONST( CanIf_RxLPduConfigType, AUTOMATIC, CANIF_APPL_CONST ) RxLPduConfig =
      &(CanIf_RxPduConfigPtr[RxPduId]);

  /* --- wakeup validation ----------------------------------------------- */
#if( CANIF_WAKEUP_VALIDATION_API == STD_ON )

  SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#if( CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT == STD_OFF )
  /* !LINKSTO CanIf.EB.CANIF403, 2 */ /* !LINKSTO CanIf.EB.CANIF680, 2 */
  /* Set the wakeup validation flag of the controller this HRH belongs to */
  if(    ((CanIf_CanControllerFlags[ControllerId] & CANIF_WAKEUP_MASK) != 0U)
      || (CanIf_CanControllerMode[ControllerId] == CANIF_CS_SLEEP)
  )
#else
  /* !LINKSTO CanIf.EB.SWS_CANIF_00286_2, 1 */
  if(CanIf_CanControllerMode[ControllerId] == CANIF_CS_STARTED)
#endif /* CANIF_VALIDATE_WAKEUP_ON_STARTED_CTRL_ONLY_SUPPORT */
  {
#if( CANIF_WAKEUP_CHECK_VALID_BY_NM == STD_ON )
    /* !LINKSTO CanIf.EB.SWS_CANIF_00286_1, 1 */
    if(    (RxLPduConfig->userType != CANIF_NO_UPPER_LAYER_CALLBACK)
        && (RxLPduConfig->userType == CANIF_NM_UPPER_LAYER_INDEX)
    )
#endif /* CANIF_WAKEUP_CHECK_VALID_BY_NM */
    {

      DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])|(CANIF_WAKEUP_VALIDATION_MASK)));
      CanIf_CanControllerFlags[ControllerId] |= CANIF_WAKEUP_VALIDATION_MASK;
    }
  }
  SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
#endif /* CANIF_WAKEUP_VALIDATION_API */


  /* --- check channel mode ---------------------------------------------- */
  if( !CANIF_CONDITION_RX_NOT_ALLOWED( ControllerId ) )
  {

    PduInfoType PduInfo;            /* PDU information */

#if (CANIF_DLC_CHECK_ENABLED == STD_ON)
    const Std_ReturnType DlcCheckResult = CanIf_DlcHandling((uint8)PduInfoPtr->SduLength,RxLPduConfig,&PduInfo);

    if(DlcCheckResult == E_OK)
#else /* CANIF_DLC_CHECK_ENABLED == STD_ON */
      PduInfo.SduLength = (uint8)PduInfoPtr->SduLength;
#endif /* (CANIF_DLC_CHECK_ENABLED == STD_ON) */
    {
      const uint8 cfgPduLength = RxLPduConfig->limitedPduLength;

      /* cut length of PDU to the length as configured in EcuC */
      if( PduInfo.SduLength > (PduLengthType) cfgPduLength )
      {
        PduInfo.SduLength = (PduLengthType) cfgPduLength;
      }

      PduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;

      /* --- write Rx buffer and notification ------------------------------ */
#if(( ( CANIF_READRXPDU_DATA_API == STD_ON ) || \
( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON ) ) || \
( CANIF_PUBLIC_PN_SUPPORT == STD_ON ))

      /* enter critical section */
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#if( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
      {
        CanIf_LPduIndexType NotifyIdx = RxLPduConfig->notifyIndex; /* notification index */

        if( NotifyIdx != CANIF_INVALID_LPDU_INDEX )
        {
          /* set receive indication flag */
          CANIF_SET_NOTIFICATION_FLAG( CanIf_RxLPduNotifyFlags, NotifyIdx );
        }
      }
#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */

#if( CANIF_READRXPDU_DATA_API == STD_ON )
      /* write Rx buffer */
      CanIf_WriteRxBuffer( RxLPduConfig, &PduInfo );
#endif /* CANIF_READRXPDU_DATA_API == STD_ON */


#if( CANIF_PUBLIC_PN_SUPPORT == STD_ON )
      /* CANIF896: Disable partial networking filter.*/
      /* !LINKSTO CanIf.ASR43.SWS_CANIF_00896, 1 */
      if((CanIf_CanControllerFlags[ControllerId] & (uint8)(CANIF_PN_FILTER_ACTIVE_MASK)) != 0)
      {
        DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId])&((uint8)(~CANIF_PN_FILTER_ACTIVE_MASK))));
        CanIf_CanControllerFlags[ControllerId] &= (uint8)(~CANIF_PN_FILTER_ACTIVE_MASK);
      }
#endif /* CANIF_PUBLIC_PN_SUPPORT == STD_ON */

      /* leave critical section */
      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

#endif /* ( CANIF_READRXPDU_DATA_API == STD_ON ) || \
  ( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON ) || \
  ( CANIF_PUBLIC_PN_SUPPORT == STD_ON ) */

#if( CANIF_HOOK_ON_RECEPTION_SUPPORT == STD_ON )
      /* !LINKSTO CanIf.EB.HookOnReception.FunctionName,1 */
      CANIF_HOOK_ON_RECEPTION_FUNCTION_NAME(CanId, &PduInfo, ControllerId);
#endif

      /* --- call receive indication callback function --------------------- */
#if( (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) )
      CanIf_SendRxIndication
      (
          RxLPduConfig->userType,
          RxLPduConfig->targetPduId,
          &PduInfo,
          CanId
      );
#elif ( !(CANIF_HOOK_ON_RECEPTION_SUPPORT == STD_ON) )
      TS_PARAM_UNUSED(CanId);
#endif /* (CANIF_UL_RX_INDICATION_SUPPORT == STD_ON) || (CANIF_CDD_RX_INDICATION_SUPPORT == STD_ON) */
    }
  }

}

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/* !LINKSTO CanIf.ASR44.SWS_CANIF_00911.Availability,1 */
FUNC( Std_ReturnType, CANIF_CODE) CanIf_EnableBusMirroring
(
  uint8 ControllerId,
  boolean MirroringActive
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_CANIF_ENABLEBUSMIRRORING_ENTRY(ControllerId, MirroringActive);

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_ENABLEBUSMIRRORING,
                            CANIF_E_UNINIT
                          );
  }
  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00912,1 */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_ENABLEBUSMIRRORING,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    if(FALSE == MirroringActive)
    {
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId]) & ((uint8) ~CANIF_BUS_MIRRORING_MASK)));
      CanIf_CanControllerFlags[ControllerId] &= (uint8)~CANIF_BUS_MIRRORING_MASK;

      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      CanIf_ResetMirrBuff(ControllerId);
    }
    else
    {
      SchM_Enter_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();

      DBG_CANIF_CANCONTROLLERFLAGS_GRP(ControllerId,(CanIf_CanControllerFlags[ControllerId]),((CanIf_CanControllerFlags[ControllerId]) | (CANIF_BUS_MIRRORING_MASK)));
      CanIf_CanControllerFlags[ControllerId] |= CANIF_BUS_MIRRORING_MASK;

      SchM_Exit_CanIf_SCHM_CANIF_EXCLUSIVE_AREA_0();
    }
    /* !LINKSTO CanIf.ASR44.SWS_CANIF_91005.ReturnValue,1 */
    retVal = E_OK;
  }

  DBG_CANIF_ENABLEBUSMIRRORING_EXIT(retVal, ControllerId, MirroringActive);

  return retVal;
}

FUNC( Std_ReturnType, CANIF_CODE) CanIf_GetControllerErrorState
(
  uint8 ControllerId,
  Can_ErrorStateType * ErrorStatePtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_CANIF_GETCONTROLLERERRORSTATE_ENTRY(ControllerId, ErrorStatePtr);

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERERRORSTATE,
                            CANIF_E_UNINIT
                          );
  }
  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00898,1 */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERERRORSTATE,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }
  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00899,1 */
  else if (NULL_PTR == ErrorStatePtr)
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERERRORSTATE,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else
#endif /* CANIF_DET_REPORT_ERROR == STD_ON */
  {
#if (CANIF_CAN_GET_CTRL_STATE_SUPPORTED == STD_ON)
    /* !LINKSTO CanIf.ASR44.SWS_CANIF_91001.ReturnValue.ProperlySupported,1 */
    /* Call Can_GetControllerErrorState() */
    retVal = CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].GetControllerErrorState(CANIF_GET_CANDRV_CTRL_ID(ControllerId), ErrorStatePtr);
#else
#if (CANIF_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(ControllerId);
#endif
    /* !LINKSTO CanIf.ASR44.SWS_CANIF_91001.ReturnValue.NotSupportedByDriver,1 */
    retVal = E_OK;
    *ErrorStatePtr = CANIF_CAN_ERROR_STATE_DUMMY;
#endif /* CANIF_CAN_GET_CTRL_STATE_SUPPORTED */
  }

  DBG_CANIF_GETCONTROLLERERRORSTATE_EXIT(retVal, ControllerId, ErrorStatePtr);
  return retVal;
}

FUNC (Std_ReturnType, CANIF_CODE) CanIf_GetControllerTxErrorCounter
(
  uint8 ControllerId,
  uint8* TxErrorCounterPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_ENTRY(ControllerId, TxErrorCounterPtr);

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* initialization */
  if( CanIf_State == CANIF_UNINITIALIZED )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERTXERRORCOUNTER,
                            CANIF_E_UNINIT
                          );
  }
  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00909,1 */
  else if( ControllerId >= CANIF_GET_NR_OF_CAN_CTRLS() )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERTXERRORCOUNTER,
                            CANIF_E_PARAM_CONTROLLERID
                          );
  }
  /* !LINKSTO CanIf.ASR44.SWS_CANIF_00910,1 */
  else if( NULL_PTR == TxErrorCounterPtr )
  {
    /* report DET error */
    CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_GETCONTROLLERTXERRORCOUNTER,
                            CANIF_E_PARAM_POINTER
                          );
  }
  else
#endif
  {
#if (CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED == STD_ON)
    /* !LINKSTO CanIf.ASR44.SWS_CANIF_91004.ReturnValue.ProperlySupported,1 */
    retVal = CanIf_CanDrvFctConfig[CANIF_GET_CAN_DRIVER_IDX(ControllerId)].GetControllerTxErrorCounter(CANIF_GET_CANDRV_CTRL_ID(ControllerId), TxErrorCounterPtr);
#else
#if (CANIF_DEV_ERROR_DETECT == STD_OFF)
    TS_PARAM_UNUSED(ControllerId);
#endif
    /* !LINKSTO CanIf.ASR44.SWS_CANIF_91004.ReturnValue.NotSupportedByDriver,1 */
    retVal = E_OK;
    *TxErrorCounterPtr = CANIF_CAN_DRV_TX_ERR_CNT_DUMMY;
#endif /* CANIF_CAN_DRV_TX_ERR_CNT_SUPPORTED == STD_ON */

  }

  DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_EXIT(retVal, ControllerId, TxErrorCounterPtr);
  return retVal;
}

STATIC FUNC(void, CANIF_CODE) CanIf_ResetMirrBuff
(
  uint8 ControllerId
)
{
  CanIf_LPduIndexType i;
#if(CANIF_SINGLE_CAN_CTRL_OPT == STD_ON)
  TS_PARAM_UNUSED( ControllerId );
#endif

  for(i = 0U; i < CanIf_ConfigPtr->nrOfTxLPdus; i++)
  {
#if(CANIF_SINGLE_CAN_CTRL_OPT == STD_OFF)
    if(ControllerId == CanIf_TxPduConfigPtr[i].intCtrlIdx)
#endif
    {
      CanIf_MirrorBuff[i].status = CANIF_FRAME_MIRROR_DONE;
    }
  }
}

#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

STATIC FUNC(PduLengthType, CANIF_CODE) CanIf_ProcessTxLength
(
    PduIdType CanIfTxPduId,
    PduLengthType RequestedTxLength
)
{
  uint8 truncOpt = CanIf_TxPduConfigPtr[CanIfTxPduId].truncateOptions;
  PduLengthType MaxFrameLength = CANIF_MAXIMUM_CAN_20_DLC;
  PduLengthType TransmitLength = CANIF_INVALID_DLC;

  /* Check if PDU is CAN FD */
  if (CANIF_CANID_IS_FD(CanIf_TxPduConfigPtr[CanIfTxPduId].canId))
  {
    /* Set the max frame length to 64 */
    MaxFrameLength = CANIF_MAXIMUM_CAN_FD_DLC;
  }

  /* Check if the requested length is greater than the configured PDU length */
  if (RequestedTxLength > CanIf_TxPduConfigPtr[CanIfTxPduId].PduLength)
  {
    /* Check if truncation is enabled for this PDU */
    if (0u != (truncOpt & CANIF_TX_PDU_TRUNCATION_MASK))
    {
      /* Check if truncation to frame is enabled for this PDU */
      if (0u != (truncOpt & CANIF_TX_PDU_TRUNCATE_TO_FRAME_MASK))
      {
        /* Check if the requested length is greater than the max frame length */
        if (RequestedTxLength > MaxFrameLength)
        {
          /* !LINKSTO CanIf.EB.SWS_CANIF_00894.TruncateToFrame,1 */
          /* Set the transmitting length to the max frame length */
          TransmitLength = MaxFrameLength;

          /* Legacy behavior */
#if( CANIF_DEV_ERROR_DETECT == STD_ON )
          /* report DET error */
          CANIF_DET_REPORT_ERROR( CANIF_MODULE_ID,
                                CANIF_INSTANCE_ID,
                                CANIF_SID_TRANSMIT,
                                CANIF_E_PARAM_DLC
                            );
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
        }
        else
        {
          /* Requested length is OK for transmission */
          TransmitLength = RequestedTxLength;
        }
      }
      else
      {
        /* !LINKSTO CanIf.EB.SWS_CANIF_00894.TruncateToPduLength,1 */
        /* Set the transmitting length to the configured PDU length */
        TransmitLength = CanIf_TxPduConfigPtr[CanIfTxPduId].PduLength;
      }
    }
    else
    {
      /* !LINKSTO CanIf.ASRR20-11.SWS_CANIF_00900,1 */
      /* report DET runtime error */
      CANIF_DET_REPORT_RUNTIME_ERROR( CANIF_MODULE_ID,
                            CANIF_INSTANCE_ID,
                            CANIF_SID_TRANSMIT,
                            CANIF_E_TXPDU_LENGTH_EXCEEDED
                        );
    }
  }
  else
  {
    /* Requested length is OK for transmission */
    TransmitLength = RequestedTxLength;
  }

  return TransmitLength;
}

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>

/*==================[end of file]============================================*/
