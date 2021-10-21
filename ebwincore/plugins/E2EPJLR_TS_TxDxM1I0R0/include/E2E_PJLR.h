/**
 * \file
 *
 * \brief AUTOSAR E2EPJLR
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPJLR.
 *
 * \version 1.0.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef E2E_PJLR_H
#define E2E_PJLR_H

/*==================[inclusions]=============================================*/

#include <E2E.h>                /* AUTOSAR E2E library */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EPJLR_VENDOR_ID) /* to prevent double declaration */
#error E2EPJLR_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EPJLR_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EPJLR_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPJLR_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EPJLR_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EPJLR_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EPJLR_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EPJLR_AR_RELEASE_MINOR_VERSION  3U

#if (defined E2EPJLR_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EPJLR_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EPJLR_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EPJLR_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EPJLR_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EPJLR_SW_MAJOR_VERSION  1U

#if (defined E2EPJLR_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EPJLR_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EPJLR_SW_MINOR_VERSION  0U

#if (defined E2EPJLR_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EPJLR_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EPJLR_SW_PATCH_VERSION  7U

/*==================[type definitions]=======================================*/

/**
 * \brief Inclusion modes of Data ID for E2E Profile JLR.
 *
 * The Data ID is two bytes long in E2E Profile JLR. There are two inclusion modes how
 * the implicit two-byte Data ID is included in the one-byte CRC.
 */
 /* CheckC disabled. E2E_PJLRDataIDMode must not be renamed to be consistent with E2E Library spec */
 /* CHECK: RULE 505 OFF */
typedef enum {
   E2E_PJLR_DATAID_BOTH = 0U,
   /**< Two bytes are included in the CRC .*/
   E2E_PJLR_DATAID_NIBBLE = 3U
   /**< The low byte is included in the implicit CRC calculation,
        the low nibble of the high byte is explicitly transmitted along with
        the data, the high nibble of the high byte is not used. 0x0 */
} E2E_PJLRDataIDMode;
/* CHECK: RULE 505 ON */

/**
 * \brief Non-modifiable configuration of the data element sent over an RTE port, for E2E Profile.
 *
 * The position of the counter and CRC is not configurable in Profile JLR.
 * Configuration of transmitted Data (Data Element or I-PDU), for E2E Profile JLR. For
 * each transmitted Data, there is an instance of this typedef.
 */
typedef struct {
    uint16 DataLength;
    /**< Length of data in bits.
     * The value shall be a multiple of 8 and <= 240. */
    uint16 DataID;
    /**< A system-unique identifier of the Data. */
    uint8 MaxDeltaCounter;
    /**< Maximum allowed gap between two counter values of two consecutively received valid Data. */
    E2E_PJLRDataIDMode DataIDMode;
    /**< Inclusion mode of Data ID in CRC computation. */
    uint16 CRCOffset;
    /**< Bit offset of CRC (i.e. since *Data) in MSB first order. In variants JLRA and JLRB, CRCOffset
    is 0. The offset shall be a multiple of 8. */
    uint16 CounterOffset;
    /**< Bit offset of Counter in MSB first order. In variants JLRA and JLRB, CounterOffset is 8.
    The offset shall be a multiple of 4. */
    uint16 DataIDNibbleOffset;
    /**< Bit offset of the low nibble of the high byte of Data ID. This parameter is used by
    E2E Library only if DataIDMode is set to E2E_PJLR_DATAID_NIBBLE (otherwise it is ignored by E2E Library).
    For DataIDMode different than E2E_PJLR_DATAID_NIBBLE, DataIDNibbleOffset shall be initialized
    to 0 (even if it is ignored by E2E Library).
    Bit offset of DataIDNibble in MSB first order. In variants JLRA and JLRB, DataIDNibbleOffset is 12.
    The DataIDNibbleOffset shall be a multiple of 4. */
} E2E_PJLRConfigType;

/**
 * \brief State of the sender for a Data protected with E2E Profile JLR.
 */
typedef struct {
    uint8 Counter;
    /**< Counter to be used for protecting the Data. */
} E2E_PJLRProtectStateType;

/**
 * \brief Definition of E2E Profile JLR receiver status type.
 *
 * Result of the verification of the Data in E2E Profile JLR, determined by the Check function.
 */
typedef enum {
    E2E_PJLRSTATUS_OK = 0x00U,
    /**< New data has been correctly received. */
    E2E_PJLRSTATUS_NONEWDATA = 0x01U,
    /**< The Check function has been invoked but new Data is not available since the last call. */
    E2E_PJLRSTATUS_ERROR = 0x07U,
    /**< The data has been received according to communication medium, but the CRC is incorrect. */
    E2E_PJLRSTATUS_REPEATED = 0x08U,
    /**< New data has been correctly received, but the Counter is identical
     * to the most recent Data */
    /**< received with Status _INITIAL, _OK, or _OKSOMELOST. */
    E2E_PJLRSTATUS_OKSOMELOST = 0x20U,
    /**< New data has been correctly received, but some data in the sequence
     * have been probably lost. */
    E2E_PJLRSTATUS_WRONGSEQUENCE = 0x40U
    /**< The new data has been correctly received, but the Counter Delta is too big
     * (DeltaCounter > MaxDeltaCounter) */
} E2E_PJLRCheckStatusType;

/**
 * \brief Definition of E2E Profile JLR receiver state type.
 *
 * State of the sender for a Data protected with E2E Profile JLR.
 */
typedef struct {
    E2E_PJLRCheckStatusType Status;
    /**< Result of the verification of the Data, determined by the Check function. */
    uint8 Counter;
    /**< Counter of last valid received message. */
} E2E_PJLRCheckStateType;

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile JLR.
 *
 * Protects the array/buffer to be transmitted using the E2E Profile JLR.
 * This includes checksum calculation, handling of Sequence counter and Data ID.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Length    Length of the data in bytes.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Sender state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_PJLRProtect
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x3b}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRProtect
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
);

/**
 * \brief Check the received Data using the E2E Profile JLR.
 *
 * Checks the Data received using the E2E Profile JLR.
 * This includes CRC calculation, handling of Sequence Counter and Data ID.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in]     DataPtr   Pointer to received Data.
 * \param[in]     Length    Length of the data in bytes.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Receiver state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_PJLRCheck
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x38}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRCheck
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
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
 * \ServiceID{0x3c}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRProtectInit
(
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Initializes the check state.
 *
 * Initializes the state structure by setting the Counter to 0x0F and
 * the State to E2E_PJLRSTATUS_ERROR.
 *
 * \param[out] StatePtr  Pointer to port/data communication state.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL NULL pointer passed.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Parameter StatePtr is a valid Pointer
 *
 * \ServiceID{0x39}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRCheckInit
(
    P2VAR(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Maps the check status of Profile JLR to a generic check status.
 *
 * The function maps the check status of Profile JLR to a generic check status,
 * which can be used by E2E state machine check function.
 * The E2E Profile JLR delivers a more fine-granular status, but this is not relevant
 * for the E2E state machine.
 *
 * \param[in] CheckReturn  Return value of the E2E_PJLRCheck function.
 * \param[in] Status  Status determined by E2E_PJLRCheck function.
 *
 * \return Profile-independent status of the reception on one single Data in one cycle.
 * \retval E2E_P_OK CheckReturn is E2E_E_OK and Status is E2E_PJLRSTATUS_OK
 * or E2E_PJLRSTATUS_OKSOMELOST.
 * \retval E2E_P_ERROR CheckReturn is E2E_E_OK and Status is E2E_PJLRSTATUS_ERROR
 * or CheckReturn is different than E2E_E_OK or Status is undefined.
 * \retval E2E_P_REPEATED CheckReturn is E2E_E_OK and Status is E2E_PJLRSTATUS_REPEATED.
 * \retval E2E_P_NONEWDATA CheckReturn is E2E_E_OK and Status is E2E_PJLRSTATUS_NONEWDATA.
 * \retval E2E_P_WRONGSEQUENCE CheckReturn is E2E_E_OK and Status is
 * E2E_PJLRSTATUS_WRONGSEQUENCE.
 *
 * \ServiceID{0x3a}
 * \Reentrancy{Reentrant for different status types}
 * \Synchronicity{Synchronous}
 */
extern FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PJLRMapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_PJLRCheckStatusType Status
);

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* ifndef E2E_PJLR_H */
/*==================[end of file]============================================*/
