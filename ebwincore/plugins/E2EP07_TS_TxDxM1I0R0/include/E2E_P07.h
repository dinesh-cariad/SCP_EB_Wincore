/**
 * \file
 *
 * \brief AUTOSAR E2EP07
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP07.
 *
 * \version 1.0.12
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef E2E_P07_H
#define E2E_P07_H

/*==================[inclusions]=============================================*/

#include <E2E.h>                /* AUTOSAR E2E library */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined E2EP07_VENDOR_ID) /* to prevent double declaration */
#error E2EP07_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define E2EP07_VENDOR_ID         1U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined E2EP07_AR_RELEASE_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP07_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define E2EP07_AR_RELEASE_MAJOR_VERSION  4U

#if (defined E2EP07_AR_RELEASE_MINOR_VERSION) /* to prevent double declaration */
#error E2EP07_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define E2EP07_AR_RELEASE_MINOR_VERSION  3U

#if (defined E2EP07_AR_RELEASE_REVISION_VERSION) /* to prevent double declaration */
#error E2EP07_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define E2EP07_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined E2EP07_SW_MAJOR_VERSION) /* to prevent double declaration */
#error E2EP07_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define E2EP07_SW_MAJOR_VERSION  1U

#if (defined E2EP07_SW_MINOR_VERSION) /* to prevent double declaration */
#error E2EP07_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define E2EP07_SW_MINOR_VERSION  0U

#if (defined E2EP07_SW_PATCH_VERSION) /* to prevent double declaration */
#error E2EP07_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define E2EP07_SW_PATCH_VERSION  12U

/*==================[type definitions]=======================================*/

/**
 * \brief Non-modifiable configuration of the data element sent over an RTE port, for E2E Profile.
 *
 * The position of the counter and CRC is not configurable in Profile 7.
 */
typedef struct {
    uint32 DataID;
    /**< A system-unique identifier of the Data. */
    uint32 Offset;
    /**< Bit offset of the E2E header from the beginning of the Data Array.
     *  The offset shall be a multiple of 8 and 0<=Offset<=MaxDataLength-(20*8).
     *  Example: If Offset equals 8, then the high byte of the E2E Length (32 bit) is
     *  written to Byte 1, the low Byte is written to Byte 2.
     *  Additionally, Offset + size of header (160 bits) shall be <= Length*8 */
    uint32 MinDataLength;
    /**< Minimum length of Data in bits. The value shall be >= 20*8 and <= MaxDataLength.
     * NOTE: In general, the MinDataLength shall at least include the E2EP07 header (20 bytes) itself
     * without any additional data. However, it is configurable for specific needs, if it is known
     * that certain data will be transmitted in any case, e.g. the SomeIp header if SomeIpXf is used
     * as a serializer or a length field for certain data types.
     * Example: E2EP07 (20 bytes E2E header) used in conjunction with SomeIpXf (8 bytes header) and
     * a variable-size array is transmitted (even if the current size is 0, at least one length field with
     * a minimum configurable size of 1 byte must be transmitted). In this case, the MinDataLength results
     * in 20 bytes + 8 bytes + 1 byte = 29 bytes. */
    uint32 MaxDataLength;
    /**< Maximum length of data in bits. The value shall be >= MinDataLength and <= 4096*8*1024 (4 MB).
     * NOTE: MaxDataLength is up to 4 MB (system dependent) and includes the E2EP07 header and the payload.
     * Any data to be protected by E2E and which are different from the E2EP07 header, is considered as payload.
     * Example: For E2EP07 the SomeIpXf header and also the length field for variable size arrays are considered
     * as payload.
     */
    uint32 MaxDeltaCounter;
    /**< Maximum allowed gap between two counter values of two consecutively received valid Data. */
} E2E_P07ConfigType;

/**
 * \brief State of the sender for a Data protected with E2E Profile 7.
 */
typedef struct {
    uint32 Counter;
    /**< Counter to be used for protecting the next Data. */
} E2E_P07ProtectStateType;

/**
 * \brief Definition of E2E Profile 7 receiver status type.
 *
 * Result of the verification of the Data in E2E Profile 7, determined by the Check function.
 */
typedef enum {
    E2E_P07STATUS_OK = 0x0U,
    /**< New data has been correctly received. */
    E2E_P07STATUS_NONEWDATA = 0x1U,
    /**< The Check function has been invoked but new Data is not available since the last call. */
    E2E_P07STATUS_ERROR = 0x2U,
    /**< The data has been received according to communication medium, but the CRC is incorrect. */
    E2E_P07STATUS_REPEATED = 0x8U,
    /**< New data has been correctly received, but the Counter is identical
     * to the most recent Data */
    E2E_P07STATUS_OKSOMELOST = 0x20U,
    /**< New data has been correctly received, but some data in the sequence
     * have been probably lost. */
    E2E_P07STATUS_WRONGSEQUENCE = 0x40U
    /**< The new data has been correctly received, but the Counter Delta is too big
     * (DeltaCounter > MaxDeltaCounter) */
} E2E_P07CheckStatusType;

/**
 * \brief Definition of E2E Profile 7 receiver state type.
 *
 * State of the sender for a Data protected with E2E Profile 7.
 */
typedef struct {
    E2E_P07CheckStatusType Status;
    /**< Result of the verification of the Data, determined by the Check function. */
    uint32 Counter;
    /**< Counter of the data in previous cycle. */
} E2E_P07CheckStateType;

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile 7.
 *
 * Protects the array/buffer to be transmitted using the E2E Profile 7.
 * This includes checksum calculation, handling of Sequence counter and Data ID.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be transmitted.
 * \param[in]     Length    Length of the data in bytes.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Sender state variable \a State has to be initialized before first call
 * \pre Referenced data \a Data is persistent during invocation of E2E_P07Protect
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x21}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P07Protect
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
);

/**
 * \brief Check the received Data using the E2E Profile 7.
 *
 * Checks the Data received using the E2E Profile 7.
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
 * \pre Referenced data \a Data is persistent during invocation of E2E_P07Check
 *      (i.e. data does not change)
 * \pre Parameter Config is a valid Pointer
 * \pre Parameter State is a valid Pointer
 * \pre Parameter Data is a valid Pointer
 *
 * \ServiceID{0x23}
 * \Reentrancy{Reentrant for different communication data / states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P07Check
(
    P2CONST(E2E_P07ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint32 Length
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
 * \ServiceID{0x22}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P07ProtectInit
(
    P2VAR(E2E_P07ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Initializes the check state.
 *
 * Initializes the state structure by setting the Counter to 0xFFFFFFFF and
 * the State to E2E_P07STATUS_ERROR.
 *
 * \param[out] StatePtr  Pointer to port/data communication state.
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL NULL pointer passed.
 * \retval E2E_E_OK Function completed successfully.
 *
 * \pre Parameter StatePtr is a valid Pointer
 *
 * \ServiceID{0x24}
 * \Reentrancy{Reentrant for different states}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, E2E_CODE) E2E_P07CheckInit
(
    P2VAR(E2E_P07CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Maps the check status of Profile 7 to a generic check status.
 *
 * The function maps the check status of Profile 7 to a generic check status,
 * which can be used by E2E state machine check function.
 * The E2E Profile 7 delivers a more fine-granular status, but this is not relevant
 * for the E2E state machine.
 *
 * \param[in] CheckReturn  Return value of the E2E_P07Check function.
 * \param[in] Status  Status determined by E2E_P07Check function.
 *
 * \return Profile-independent status of the reception on one single Data in one cycle.
 * \retval E2E_P_OK CheckReturn is E2E_E_OK and Status is E2E_P07STATUS_OK
 * or E2E_P07STATUS_OKSOMELOST.
 * \retval E2E_P_ERROR CheckReturn is E2E_E_OK and Status is E2E_P07STATUS_ERROR
 * or CheckReturn is different than E2E_E_OK or Status is undefined.
 * \retval E2E_P_REPEATED CheckReturn is E2E_E_OK and Status is E2E_P07STATUS_REPEATED
 * \retval E2E_P_NONEWDATA CheckReturn is E2E_E_OK and Status is E2E_P07STATUS_NONEWDATA.
 * \retval E2E_P_WRONGSEQUENCE CheckReturn is E2E_E_OK and Status is
 * E2E_P07STATUS_WRONGSEQUENCE.
 *
 * \ServiceID{0x25}
 * \Reentrancy{Reentrant for different status types}
 * \Synchronicity{Synchronous}
 */
extern FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P07MapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_P07CheckStatusType Status
);

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

#endif /* ifndef E2E_P07_H */
/*==================[end of file]============================================*/
