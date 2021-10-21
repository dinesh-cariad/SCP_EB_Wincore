/**
 * \file
 *
 * \brief AUTOSAR E2EP02
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP02.
 *
 * \version 2.1.17
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef E2E_P02_H
#define E2E_P02_H

/*==================[inclusions]=============================================*/

#include <E2E.h>                /* AUTOSAR E2E library */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EP02_VENDOR_ID) /* to prevent double declaration */
#error E2EP02_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EP02_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EP02_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP02_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EP02_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EP02_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EP02_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EP02_AR_RELEASE_MINOR_VERSION  2U

#if (defined E2EP02_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EP02_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EP02_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR specification version identification]----------*/

#if (defined E2EP02_AR_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP02_AR_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR specification major version */
#define E2EP02_AR_MAJOR_VERSION  4U

#if (defined E2EP02_AR_MINOR_VERSION) /* to prevent double declaration */
#error E2EP02_AR_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR specification minor version */
#define E2EP02_AR_MINOR_VERSION  2U

#if (defined E2EP02_AR_PATCH_VERSION) /* to prevent double declaration */
#error E2EP02_AR_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR specification patch version */
#define E2EP02_AR_PATCH_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EP02_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP02_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EP02_SW_MAJOR_VERSION  2U

#if (defined E2EP02_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EP02_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EP02_SW_MINOR_VERSION  1U

#if (defined E2EP02_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EP02_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EP02_SW_PATCH_VERSION  17U

/*==================[type definitions]=======================================*/

/**
 * \brief Non-modifiable configuration of the data element sent over an RTE port, for E2E Profile.
 *
 * The position of the counter and CRC is not configurable in Profile 2.
 */
typedef struct {
   uint16 DataLength;
   /**< Length of the data in bits. The value shall be a multiple of 8 and shall be <= 2048. */
   uint8  DataIDList[16];
   /**< An array of appropriately chosen Data IDs for protection against masquerading. */
   uint8  MaxDeltaCounterInit;
   /**< Initial maximum allowed gap between two counter values of two consecutively
    * received valid Data. */
   uint8  MaxNoNewOrRepeatedData;
   /**< Maximum amount of missing or repeated Data which the receiver does not expect
    * to exceed under normal communication conditions. */
   uint8  SyncCounterInit;
   /**< Number of Data required for validating the consistency of the counter
    * that must be received with a valid counter after the detection of an unexpected behavior
    * of a received counter. */
   uint16 Offset;
   /**< Offset of the E2E header in the Data[] array in bits.
    * It shall be 0 ≤ Offset ≤ MaxDataLength-(2*8) Since there is limitation for this A4.2.1.
    * It shall be configured as Zero always.*/
} E2E_P02ConfigType;

/**
 * \brief State of the sender for a Data protected with E2E Profile 2.
 */
typedef struct {
   uint8 Counter;
   /**< Counter to be used for protecting the Data. The counter is incremented modulo 16 */
} E2E_P02SenderStateType;

/**
 * \brief Definition of E2E Profile 2 receiver status type.
 *
 * Result of the verification of the Data in E2E Profile 2, determined by the Check function.
 */
typedef enum {
   E2E_P02STATUS_OK = 0x0U,
   /**< New data has been correctly received. */
   E2E_P02STATUS_NONEWDATA = 0x1U,
   /**< The Check function has been invoked but new Data is not available since the last call. */
   E2E_P02STATUS_WRONGCRC = 0x2U,
   /**< The data has been received according to communication medium, but the CRC is incorrect. */
   E2E_P02STATUS_SYNC = 0x3U,
   /**< New data has been received after detection of an unexpected behavior of counter. */
   E2E_P02STATUS_INITIAL = 0x4U,
   /**< New data has been correctly received, but this is the first Data since the receiver's
    * (re-)initialization. */
   E2E_P02STATUS_REPEATED = 0x8U,
   /**< New data has been correctly received, but the Counter is identical
    * to the most recent Data */
   /**< received with Status _INITIAL, _OK, or _OKSOMELOST. */
   E2E_P02STATUS_OKSOMELOST = 0x20U,
   /**< New data has been correctly received, but some data in the sequence
    * have been probably lost. */
   E2E_P02STATUS_WRONGSEQUENCE = 0x40U
   /**< The new data has been correctly received, but the Counter Delta is too big
    * (DeltaCounter > MaxDeltaCounter) */
} E2E_P02ReceiverStatusType;

/**
 * \brief Definition of E2E Profile 2 receiver state type.
 *
 * State of the sender for a Data protected with E2E Profile 2.
 */
typedef struct {
   E2E_P02ReceiverStatusType Status;
   /**< Result of the verification of the Data, determined by the Check function. */
   uint8   LastValidCounter;
   /**< Counter of last valid received message. */
   uint8   MaxDeltaCounter;
   /**< MaxDeltaCounter specifies the maximum allowed difference between two counter values of */
   /**< consecutively received valid messages. */
   boolean WaitForFirstData;
   /**< If true means that no correct data (with correct Data ID and CRC) has been yet received */
   /**< after the receiver (re-)initialization. */
   boolean NewDataAvailable;
   /**< Indicates to E2E Library that a new data is available for the Library to be checked. */
   /**< This attribute is set by the E2E Library caller and not by the E2E Library. */
   uint8   LostData;
   /**< Number of data (messages) lost since reception of last valid one. */
   uint8   NoNewOrRepeatedDataCounter;
   /**< Amount of consecutive reception cycles in which either (1) there was no new data,
    * or (2) when the data was repeated. */
   uint8   SyncCounter;
   /**< Number of consecutively received valid Data required for determining the
    * consistency of the counter. */
} E2E_P02ReceiverStateType;

/**
 * \brief Definition of E2E Profile 2 protect state type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2.1.
 *
 */
typedef E2E_P02SenderStateType E2E_P02ProtectStateType;

/**
 * \brief Definition of E2E Profile 2 check state type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2.1.
 *
 */
typedef E2E_P02ReceiverStateType E2E_P02CheckStateType;

/**
 * \brief Definition of E2E Profile 2 check status type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2..
 *
 */
typedef E2E_P02ReceiverStatusType E2E_P02CheckStatusType;


/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile 2.
 *
 * Protects the array/buffer to be transmitted using the E2E Profile 2.
 * This includes checksum calculation, handling of Sequence counter and Data ID.
 *
 * \param[in]    ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Protect state variable \a StatePtr has to be initialized before first call
 * \pre Referenced data \a DataPtr is persistent during invocation of E2E_P02Protect
 *      (i.e. data does not change)
 * \pre Parameter ConfigPtr is a valid Pointer
 * \pre Parameter StatePtr is a valid Pointer
 * \pre Parameter DataPtr is a valid Pointer
 *
 * \ServiceID{0x03}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02Protect
(
   P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
   P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);

/**
 * \brief Check the received Data using the E2E Profile 2.
 *
 * Checks the Data received using the E2E Profile 2.
 * This includes CRC calculation, handling of Sequence Counter and Data ID.
 *
 * \param[in]    ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in]    DataPtr   Pointer to received Data.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Check state variable \a StatePtr has to be initialized before first call
 * \pre Referenced data \a DataPtr is persistent during invocation of E2E_P02Check
 *      (i.e. data does not change)
 * \pre Parameter ConfigPtr is a valid Pointer
 * \pre Parameter StatePtr is a valid Pointer
 * \pre Parameter DataPtr is a valid Pointer
 *
 * \ServiceID{0x04}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02Check
(
   P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
   P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);
/**
 * \brief Initializes the protection state.
 *
 * Initializes the state structure by setting the Counter to 0.
 *
 * \param[out] StatePtr  Pointer to port/data communication state.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL NULL pointer passed.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Parameter StatePtr is a valid Pointer
 *
 * \ServiceID{0x1e}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02ProtectInit
(
   P2VAR(E2E_P02ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Initializes the check state.
 *
 * Initializes the state structure by setting:
 * LastValidCounter = 0
 * MaxDeltaCounter = 0
 * WaitForFirstData = E2EXF_WAIT_FOR_FIRST_DATA_INIT_VALUE
 * NewDataAvailable = TRUE
 * LostData = 0
 * Status = E2E_P02STATUS_NONEWDATA
 * NoNewOrRepeatedDataCounter = 0
 * SyncCounter = 0.
 *
 * \param[out] StatePtr  Pointer to port/data communication state.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL NULL pointer passed.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Parameter StatePtr is a valid Pointer
 *
 * \ServiceID{0x1f}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P02CheckInit
(
   P2VAR(E2E_P02CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Maps the check status of Profile 2 to a generic check status.
 *
 * The function maps the check status of Profile 2 to a generic check status,
 * which can be used by E2E state machine check function.
 * The E2E Profile 2 delivers a more fine-granular status, but this is not relevant
 * for the E2E state machine.
 *
 * \param[in] CheckReturn  Return value of the E2E_P02Check function.
 * \param[in] Status  Status determined by E2E_P02Check function.
 * \param[in] profileBehavior  FALSE: check has the legacy behavior, before R4.2
 *                             TRUE: check behaves like new P4/P5/P6 profiles introduced in R4.2
 *
 * \return Profile-independent status of the reception on one single Data in one cycle.
 * \retval E2E_P_OK CheckReturn is E2E_E_OK and profileBehavior is TRUE and Status is
 *         E2E_P02STATUS_OK or E2E_P02STATUS_OKSOMELOST or E2E_P02STATUS_SYNC, or
 *         CheckReturn is E2E_E_OK and profileBehavior is FALSE and Status is
 *         E2E_P02STATUS_OK or E2E_P02STATUS_OKSOMELOST or E2E_P02STATUS_INITIAL.
 * \retval E2E_P_ERROR CheckReturn is E2E_E_OK and Status is E2E_P02STATUS_WRONGCRC, or
 *         CheckReturn is E2E_E_OK and Status is a not defined status, or
 *         CheckReturn does not equal E2E_E_OK.
 * \retval E2E_P_REPEATED CheckReturn is E2E_E_OK and Status is E2E_P02STATUS_REPEATED.
 * \retval E2E_P_NONEWDATA CheckReturn is E2E_E_OK and Status is E2E_P02STATUS_NONEWDATA.
 * \retval E2E_P_WRONGSEQUENCE CheckReturn is E2E_E_OK and profileBehavior is TRUE
 *         and Status is E2E_P02STATUS_WRONGSEQUENCE or E2E_P02STATUS_INITIAL, or
 *         CheckReturn is E2E_E_OK and profileBehavior is FALSE
 *         and Status is E2E_P02STATUS_WRONGSEQUENCE or E2E_P02STATUS_SYNC.
 *
 * \ServiceID{0x20}
 * \Reentrancy{Reentrant for different status types}
 * \Synchronicity{Synchronous}
 */
extern FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P02MapStatusToSM
(
   Std_ReturnType CheckReturn,
   E2E_P02CheckStatusType Status,
   boolean profileBehavior
);
#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* ifndef E2E_P02_H */
/*==================[end of file]============================================*/
