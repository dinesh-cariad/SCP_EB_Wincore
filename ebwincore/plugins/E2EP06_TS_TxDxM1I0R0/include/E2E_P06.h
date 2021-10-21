/**
 * \file
 *
 * \brief AUTOSAR E2EP06
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP06.
 *
 * \version 1.0.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef E2E_P06_H
#define E2E_P06_H

/*==================[inclusions]=============================================*/

#include <E2E.h>                /* AUTOSAR E2E library */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EP06_VENDOR_ID) /* to prevent double declaration */
#error E2EP06_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EP06_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EP06_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP06_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EP06_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EP06_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EP06_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EP06_AR_RELEASE_MINOR_VERSION  2U

#if (defined E2EP06_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EP06_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EP06_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EP06_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP06_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EP06_SW_MAJOR_VERSION  1U

#if (defined E2EP06_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EP06_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EP06_SW_MINOR_VERSION  0U

#if (defined E2EP06_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EP06_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EP06_SW_PATCH_VERSION  11U

/*==================[type definitions]=======================================*/

/**
 * \brief Non-modifiable configuration of the data element sent over an RTE port, for E2E Profile.
 *
 * The position of the counter and CRC is not configurable in Profile 6.
 */
typedef struct {
    uint16 Offset;
    /**< Bit offset of the E2E header from the beginning of the Data Array.
     *  The offset shall be a multiple of 8 and 0<=Offset<=MaxDataLength-(5*8).
     *  Example: If Offset equals 8, then the high byte of the E2E Crc (16 bit) is
     *  written to Byte 1, the low Byte is written to Byte 2.
     *  Additionally, Offset + size of header (40 bits) shall be <= Length*8 */
    uint16 MinDataLength;
    /**< Minimum length of Data in bits. The value shall be a multiple of 8 and >= 5*8 and
    ≤ 4096*8 (4kB). */
    uint16 MaxDataLength;
    /**< Maximum length of data in bits.
     * The value shall be a multiple of 8 and >= MinDataLength and <= 4096*8 (4kB).*/
    uint16 DataID;
    /**< A system-unique identifier of the Data. */
    uint8 MaxDeltaCounter;
    /**< Maximum allowed gap between two counter values of two consecutively received valid Data. */
} E2E_P06ConfigType;

/**
 * \brief State of the sender for a Data protected with E2E Profile 6.
 */
typedef struct {
    uint8 Counter;
    /**< Counter to be used for protecting the Data. */
} E2E_P06ProtectStateType;

/**
 * \brief Definition of E2E Profile 6 receiver status type.
 *
 * Result of the verification of the Data in E2E Profile 6, determined by the Check function.
 */
typedef enum {
    E2E_P06STATUS_OK = 0x00U,
    /**< New data has been correctly received. */
    E2E_P06STATUS_NONEWDATA = 0x01U,
    /**< The Check function has been invoked but new Data is not available since the last call. */
    E2E_P06STATUS_ERROR = 0x07U,
    /**< The data has been received according to communication medium, but the CRC is incorrect. */
    E2E_P06STATUS_REPEATED = 0x08U,
    /**< New data has been correctly received, but the Counter is identical
     * to the most recent Data */
    /**< received with Status _INITIAL, _OK, or _OKSOMELOST. */
    E2E_P06STATUS_OKSOMELOST = 0x20U,
    /**< New data has been correctly received, but some data in the sequence
     * have been probably lost. */
    E2E_P06STATUS_WRONGSEQUENCE = 0x40U
    /**< The new data has been correctly received, but the Counter Delta is too big
     * (DeltaCounter > MaxDeltaCounter) */
} E2E_P06CheckStatusType;

/**
 * \brief Definition of E2E Profile 6 receiver state type.
 *
 * State of the sender for a Data protected with E2E Profile 6.
 */
typedef struct {
    E2E_P06CheckStatusType Status;
    /**< Result of the verification of the Data, determined by the Check function. */
    uint8 Counter;
    /**< Counter of last valid received message. */
} E2E_P06CheckStateType;

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile 6.
 *
 * Protects the array/buffer to be transmitted using the E2E Profile 6.
 * This includes checksum calculation, handling of Sequence counter and Data ID.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Length    Length of the Data.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Sender state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_P06Protect
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x2b}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P06Protect
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
);

/**
 * \brief Check the received Data using the E2E Profile 6.
 *
 * Checks the Data received using the E2E Profile 6.
 * This includes CRC calculation, handling of Sequence Counter and Data ID.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in]     DataPtr   Pointer to received Data.
 * \param[in]     Length    Length of the Data.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Receiver state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_P06Check
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x2d}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P06Check
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
 * \ServiceID{0x2c}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P06ProtectInit
(
    P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Initializes the check state.
 *
 * Initializes the state structure by setting the Counter to 0xFF and
 * the State to E2E_P06STATUS_ERROR.
 *
 * \param[out] StatePtr  Pointer to port/data communication state.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL NULL pointer passed.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Parameter StatePtr is a valid Pointer
 *
 * \ServiceID{0x2e}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P06CheckInit
(
    P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Maps the check status of Profile 6 to a generic check status.
 *
 * The function maps the check status of Profile 6 to a generic check status,
 * which can be used by E2E state machine check function.
 * The E2E Profile 6 delivers a more fine-granular status, but this is not relevant
 * for the E2E state machine.
 *
 * \param[in] CheckReturn  Return value of the E2E_P06Check function.
 * \param[in] Status  Status determined by E2E_P06Check function.
 *
 * \return Profile-independent status of the reception on one single Data in one cycle.
 * \retval E2E_P_OK CheckReturn is E2E_E_OK and Status is E2E_P06STATUS_OK
 * or E2E_P06STATUS_OKSOMELOST.
 * \retval E2E_P_ERROR CheckReturn is E2E_E_OK and Status is E2E_P06STATUS_ERROR
 * or CheckReturn is different than E2E_E_OK or Status is undefined.
 * \retval E2E_P_REPEATED CheckReturn is E2E_E_OK and Status is E2E_P06STATUS_REPEATED.
 * \retval E2E_P_NONEWDATA CheckReturn is E2E_E_OK and Status is E2E_P06STATUS_NONEWDATA.
 * \retval E2E_P_WRONGSEQUENCE CheckReturn is E2E_E_OK and Status is
 * E2E_P06STATUS_WRONGSEQUENCE.
 *
 * \ServiceID{0x2f}
 * \Reentrancy{Reentrant for different status types}
 * \Synchronicity{Synchronous}
 */
extern FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P06MapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_P06CheckStatusType Status
);

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* ifndef E2E_P06_H */
/*==================[end of file]============================================*/
