/**
 * \file
 *
 * \brief AUTOSAR E2EPRN
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPRN.
 *
 * \version 1.0.12
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef E2E_PRN_H
#define E2E_PRN_H

/*==================[inclusions]=============================================*/

#include <E2E.h>                /* AUTOSAR E2E library */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EPRN_VENDOR_ID) /* to prevent double declaration */
#error E2EPRN_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EPRN_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EPRN_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EPRN_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EPRN_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EPRN_AR_RELEASE_MINOR_VERSION  0U

#if (defined E2EPRN_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EPRN_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR specification version identification]----------*/

#if (defined E2EPRN_AR_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR specification major version */
#define E2EPRN_AR_MAJOR_VERSION  2U

#if (defined E2EPRN_AR_MINOR_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR specification minor version */
#define E2EPRN_AR_MINOR_VERSION  0U

#if (defined E2EPRN_AR_PATCH_VERSION) /* to prevent double declaration */
#error E2EPRN_AR_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR specification patch version */
#define E2EPRN_AR_PATCH_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EPRN_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPRN_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EPRN_SW_MAJOR_VERSION  1U

#if (defined E2EPRN_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EPRN_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EPRN_SW_MINOR_VERSION  0U

#if (defined E2EPRN_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EPRN_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EPRN_SW_PATCH_VERSION  12U

/*==================[type definitions]=======================================*/

/**
 * \brief Inclusion modes of Data ID for E2E Profile PRN.
 *
 * The Data ID is two bytes long in E2E Profile PRN. There are three inclusion modes how
 * the implicit two-byte Data ID is included in the one-byte CRC.
 */
 /* CheckC disabled. E2E_PRNDataIDMode is not renamed to keep consistency with AUTOSAR
    profiles defined in E2E Library spec */
 /* CHECK: RULE 505 OFF */
typedef enum {
   E2E_PRN_DATAID_BOTH = 0U,
   /**< Two bytes are included in the CRC .*/
   E2E_PRN_DATAID_ALT = 1U,
   /**< One of the two bytes is included, alternating high and low byte. */
   E2E_PRN_DATAID_LOW = 2U,
   /**< Only low byte is included, high byte is never used. */
   E2E_PRN_DATAID_NIBBLE = 3U
   /**< The low byte is included in the implicit CRC calculation,
        the low nibble of the high byte is explicitly transmitted along with
        the data, the high nibble of the high byte is not used. */
} E2E_PRNDataIDMode;
/* CHECK: RULE 505 ON */

/**
 * \brief Configuration of transmitted Data for E2E Profile PRN.
 *
 * Configuration of transmitted Data (Data Element or I-PDU), for E2E Profile PRN.
 * For each transmitted Data, there is an instance of this typedef.
 */
typedef struct {
   uint16 CounterOffset;
   /**< Bit offset of Counter (Least Significant Bit) from the beginning of the Byte-Array
    * (bit numbering: bit 0 is the least important). The offset shall be a multiple of 4.
    * For example, offset 8 means that the Counter will take the low nibble
    * of the byte 1, i.e. bits 8..11. */
   uint16 CRCOffset;
   /**< Bit offset of CRC (Least Significant Bit) from the beginning of the Byte-Array
    * (bit numbering: bit 0 is the least important). The offset shall be a multiple of 8.
    * For example, offset 8 means that the CRC will take the byte 1, i.e. bits 8..15. */
   uint16 DataID;
   /**< A unique identifier for protection against masquerading. */
   uint16 DataIDNibbleOffset;
   /**< Bit offset of the low nibble (Least Significant Bit) of the high byte of Data ID
    * from the beginning of the Byte-Array (bit numbering: bit 0 is the least important).
    * The offset shall be a multiple of 4. For example, offset 8 means that the DataIDNibble
    * will take the low nibble of the byte 1, i.e. bits 8..11. */
   E2E_PRNDataIDMode DataIDMode;
   /**< Inclusion mode of ID in CRC computation. */
   uint16 DataLength;
   /**< Length of the data in bits. The value shall be a multiple of 8 and shall be <= 2048. */
   uint8 MaxDeltaCounterInit;
   /**< Initial maximum allowed gap between two counter values of two consecutively
    * received valid Data. */
   uint8 MaxNoNewOrRepeatedData;
   /**< Maximum amount of missing or repeated Data which the receiver does not expect
    * to exceed under normal communication conditions. */
   uint8 SyncCounterInit;
   /**< Number of Data required for validating the consistency of the counter
    * that must be received with a valid counter after the detection of an unexpected behavior
    * of a received counter. */
} E2E_PRNConfigType;

/**
 * \brief State of the sender for a Data protected with E2E Profile PRN.
 */
typedef struct {
   uint8 Counter;
   /**< Counter to be used for protecting the Data. The counter is incremented modulo 15 */
} E2E_PRNSenderStateType;

/**
 * \brief Definition of E2E Profile PRN receiver status type.
 *
 * Result of the verification of the Data in E2E Profile PRN, determined by the Check function.
 */
typedef enum {
   E2E_PRNSTATUS_OK = 0x0U,
    /**< New data has been correctly received. */
   E2E_PRNSTATUS_NONEWDATA = 0x1U,
   /**< The Check function has been invoked but new Data is not available since the last call. */
   E2E_PRNSTATUS_WRONGCRC = 0x2U,
   /**< The data has been received according to communication medium, but the CRC is incorrect. */
   E2E_PRNSTATUS_SYNC = 0x3U,
   /**< New data has been received after detection of an unexpected behavior of counter. */
   E2E_PRNSTATUS_INITIAL = 0x4U,
   /**< New data has been correctly received, but this is the first Data since
    * the receiver's (re-)initialization. */
   E2E_PRNSTATUS_REPEATED = 0x8U,
   /**< New data has been correctly received, but the Counter is identical
    * to the most recent Data */
   /**< received with Status _INITIAL, _OK, or _OKSOMELOST. */
   E2E_PRNSTATUS_OKSOMELOST = 0x20U,
   /**< New data has been correctly received, but some data in the sequence
    * have been probably lost. */
   E2E_PRNSTATUS_WRONGSEQUENCE = 0x40U,
   /**< The new data has been correctly received, but the Counter Delta is too big
    * (DeltaCounter > MaxDeltaCounter) */
   E2E_PRNSTATUS_DATAINVALID = 0x80U
   /**< All bits in the received data except for byte 0 (CRC) are set to one. */
} E2E_PRNReceiverStatusType;

/**
 * \brief Definition of E2E Profile PRN receiver state type.
 *
 * State of the sender for a Data protected with E2E Profile PRN.
 */
typedef struct {
   E2E_PRNReceiverStatusType Status;
   /**< Result of the verification of the Data, determined by the Check function. */
   uint8 LastValidCounter;
   /**< Counter of last valid received message. */
   uint8 MaxDeltaCounter;
   /**< MaxDeltaCounter specifies the maximum allowed difference between two counter values of */
   /**< consecutively received valid messages. */
   boolean WaitForFirstData;
   /**< If true means that no correct data (with correct Data ID and CRC) has been yet received */
   /**< after the receiver (re-)initialization. */
   boolean NewDataAvailable;
   /**< Indicates to E2E Library that a new data is available for the Library to be checked. */
   /**< This attribute is set by the E2E Library caller and not by the E2E Library. */
   uint8 LostData;
   /**< Number of data (messages) lost since reception of last valid one. */
   uint8 NoNewOrRepeatedDataCounter;
   /**< Amount of consecutive reception cycles in which either (1) there was no new data,
    * or (2) when the data was repeated. */
   uint8 SyncCounter;
   /**< Number of consecutively received valid Data required for determining the
    * consistency of the counter. */
} E2E_PRNReceiverStateType;

/**
 * \brief Definition of E2E Profile PRN protect state type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2.1.
 *
 */
typedef E2E_PRNSenderStateType E2E_PRNProtectStateType;

/**
 * \brief Definition of E2E Profile PRN check state type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2.1.
 *
 */
typedef E2E_PRNReceiverStateType E2E_PRNCheckStateType;

/**
 * \brief Definition of E2E Profile PRN check status type.
 *
 * Type-definition for backward-compatibility due to type renaming done in AUTOSAR version 4.2.1.
 *
 */
typedef E2E_PRNReceiverStatusType E2E_PRNCheckStatusType;

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile PRN.
 *
 * Protects the array/buffer to be transmitted using the E2E Profile PRN.
 * This includes checksum calculation, handling of counter and Data ID.
 *
 * \param[in]     Config Pointer to static configuration.
 * \param[in,out] State  Pointer to port/data communication state.
 * \param[in,out] Data   Pointer to Data to be protected.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Sender state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_PRNProtect
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x01}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PRNProtect
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);

/**
 * \brief Check the received Data using the E2E Profile PRN.
 *
 * Checks the Data received using the E2E Profile PRN.
 * This includes CRC calculation, handling of Counter and Data ID.
 *
 * \param[in]     Config Pointer to static configuration.
 * \param[in,out] State  Pointer to port/data communication state.
 * \param[in]     Data   Pointer to received Data.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Receiver state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_PRNCheck
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PRNCheck
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
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
 * \ServiceID{0x1b}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PRNProtectInit
(
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Initializes the check state.
 *
 * Initializes the state structure by setting:
 * LastValidCounter = 0
 * MaxDeltaCounter = 0
 * WaitForFirstData = FALSE
 * NewDataAvailable = TRUE
 * LostData = 0
 * Status = E2E_PRNSTATUS_NONEWDATA
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
 * \ServiceID{0x1c}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PRNCheckInit
(
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Maps the check status of Profile PRN to a generic check status.
 *
 * The function maps the check status of Profile PRN to a generic check status,
 * which can be used by E2E state machine check function.
 * The E2E Profile PRN delivers a more fine-granular status, but this is not relevant
 * for the E2E state machine.
 *
 * \param[in] CheckReturn  Return value of the E2E_PRNCheck function.
 * \param[in] Status  Status determined by E2E_PRNCheck function.
 * \param[in] profileBehavior  FALSE: check has the legacy behavior, before R4.2
 *                             TRUE: check behaves like new P4/P5/P6 profiles introduced in R4.2
 *
 * \return Profile-independent status of the reception on one single Data in one cycle.
 * \retval E2E_P_OK CheckReturn is E2E_E_OK and profileBehavior is TRUE and Status is
 *         E2E_PRNSTATUS_OK or E2E_PRNSTATUS_OKSOMELOST or E2E_PRNSTATUS_SYNC, or
 *         CheckReturn is E2E_E_OK and profileBehavior is FALSE and Status is
 *         E2E_PRNSTATUS_OK or E2E_PRNSTATUS_OKSOMELOST or E2E_PRNSTATUS_INITIAL.
 * \retval E2E_P_ERROR CheckReturn is E2E_E_OK and Status is E2E_PRNSTATUS_WRONGCRC, or
 *         CheckReturn is E2E_E_OK and Status is a not defined status, or
 *         CheckReturn does not equal E2E_E_OK.
 * \retval E2E_P_REPEATED CheckReturn is E2E_E_OK and Status is E2E_PRNSTATUS_REPEATED.
 * \retval E2E_P_NONEWDATA CheckReturn is E2E_E_OK and Status is E2E_PRNSTATUS_NONEWDATA.
 * \retval E2E_P_WRONGSEQUENCE CheckReturn is E2E_E_OK and profileBehavior is TRUE
 *         and Status is E2E_PRNSTATUS_WRONGSEQUENCE or E2E_PRNSTATUS_INITIAL, or
 *         CheckReturn is E2E_E_OK and profileBehavior is FALSE
 *         and Status is E2E_PRNSTATUS_WRONGSEQUENCE or E2E_PRNSTATUS_SYNC.
 *
 * \ServiceID{0x1d}
 * \Reentrancy{Reentrant for different status types}
 * \Synchronicity{Synchronous}
 */
extern FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PRNMapStatusToSM
(
   Std_ReturnType CheckReturn,
   E2E_PRNCheckStatusType Status,
   boolean profileBehavior
);

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* ifndef E2E_PRN_H */
/*==================[end of file]============================================*/
