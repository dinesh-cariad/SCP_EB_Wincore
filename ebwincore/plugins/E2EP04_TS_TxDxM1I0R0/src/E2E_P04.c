/**
 * \file
 *
 * \brief AUTOSAR E2EP04
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EP04.
 *
 * \version 1.0.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <E2E_P04.h>       /* E2E Profile 4         */
#include <SCrc_CRC32P4.h>  /* SCrc routine CRC32P4  */

/*==================[macros]=================================================*/

#if (defined E2EP04_EB_CRC_STARTVALUE) /* to prevent double declaration */
#error E2EP04_EB_CRC_STARTVALUE is already defined
#endif /* #if (defined E2EP04_EB_CRC_STARTVALUE) */

/** \brief Start value for Crc calculation */
#define E2EP04_EB_CRC_STARTVALUE (uint32)0xFFFFFFFFUL

#if (defined E2EP04_EB_COUNTER_MAXVALUE) /* to prevent double declaration */
#error E2EP04_EB_COUNTER_MAXVALUE is already defined
#endif /* #if (defined E2EP04_EB_COUNTER_MAXVALUE) */

/** \brief Maximum value for state counter */
#define E2EP04_EB_COUNTER_MAXVALUE 0xFFFFU

#if (defined E2EP04_EB_E2EHEADER_LENGTH_OFFSET) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_LENGTH_OFFSET is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_LENGTH_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of Length coded. */
#define E2EP04_EB_E2EHEADER_LENGTH_OFFSET 0U

#if (defined E2EP04_EB_E2EHEADER_COUNTER_OFFSET) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_COUNTER_OFFSET is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_COUNTER_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of Counter coded. */
#define E2EP04_EB_E2EHEADER_COUNTER_OFFSET 2U

#if (defined E2EP04_EB_E2EHEADER_DATAID_OFFSET) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_DATAID_OFFSET is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_DATAID_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of DataID coded. */
#define E2EP04_EB_E2EHEADER_DATAID_OFFSET 4U

#if (defined E2EP04_EB_E2EHEADER_CRC_OFFSET) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_CRC_OFFSET is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_CRC_OFFSET) */

/** \brief Offset in bytes within the E2E header where MSB of CRC coded. */
#define E2EP04_EB_E2EHEADER_CRC_OFFSET 8U

#if (defined E2EP04_EB_E2EHEADER_CRC_SIZE) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_CRC_SIZE is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_CRC_SIZE) */

/** \brief Crc size in bytes within E2E header. */
#define E2EP04_EB_E2EHEADER_CRC_SIZE 4U

#if (defined E2EP04_EB_E2EHEADER_SIZE_BITS) /* to prevent double declaration */
#error E2EP04_EB_E2EHEADER_SIZE_BITS is already defined
#endif /* #if (defined E2EP04_EB_E2EHEADER_SIZE_BITS) */

/** \brief E2E Profile 04 header size in bits. */
#define E2EP04_EB_E2EHEADER_SIZE_BITS 96U

#if (defined E2EP04_EB_WRITE2BYTE_VALUE) /* to prevent double declaration */
#error E2EP04_EB_WRITE2BYTE_VALUE is already defined
#endif /* #if (defined E2EP04_EB_WRITE2BYTE_VALUE) */

/** \brief Write a 2-byte value in Big Endian order to the passed Data Pointer */
#define E2EP04_EB_WRITE2BYTE_VALUE(DataPtr, Value)                          \
do {                                                                        \
    (DataPtr)[0] = (uint8)((uint16)((Value) >> 8U) & (uint8)0xFFU);         \
    (DataPtr)[1] = (uint8)((Value) & (uint8)0xFFU);                         \
} while (0)

#if (defined E2EP04_EB_WRITE4BYTE_VALUE) /* to prevent double declaration */
#error E2EP04_EB_WRITE4BYTE_VALUE is already defined
#endif /* #if (defined E2EP04_EB_WRITE4BYTE_VALUE) */

/** \brief Write a 4-byte value in Big Endian order to the passed Data Pointer */
#define E2EP04_EB_WRITE4BYTE_VALUE(DataPtr, Value)                          \
do {                                                                        \
    (DataPtr)[0] = (uint8)((uint32)((Value) >> 24U) & (uint8)0xFFU);        \
    (DataPtr)[1] = (uint8)((uint32)((Value) >> 16U) & (uint8)0xFFU);        \
    (DataPtr)[2] = (uint8)((uint32)((Value) >> 8U) & (uint8)0xFFU);         \
    (DataPtr)[3] = (uint8)((Value) & (uint8)0xFFU);                         \
} while (0)


#if (defined E2EP04_EB_READ2BYTE_VALUE) /* to prevent double declaration */
#error E2EP04_EB_READ2BYTE_VALUE is already defined
#endif /* #if (defined E2EP04_EB_READ2BYTE_VALUE) */

/** \brief Read a 2-byte value in Big Endian order from the passed Data Pointer */
#define E2EP04_EB_READ2BYTE_VALUE(DataPtr)                                  \
    (                                                                       \
      (uint16)(((uint16)((DataPtr)[1]))                                     \
    | ((uint16)(((uint16)((DataPtr)[0]))<<8U)))                             \
    )                                                                       \

#if (defined E2EP04_EB_READ4BYTE_VALUE) /* to prevent double declaration */
#error E2EP04_EB_READ4BYTE_VALUE is already defined
#endif /* #if (defined E2EP04_EB_READ4BYTE_VALUE) */

/** \brief Read a 4-byte value in Big Endian order from the passed Data Pointer */
#define E2EP04_EB_READ4BYTE_VALUE(DataPtr)                                  \
    (                                                                       \
      (uint32)(((uint32)((DataPtr)[3]))                                     \
    | (uint32)(((uint32)(((uint32)((DataPtr)[2]))<<8U ))                    \
    | (uint32)(((uint32)(((uint32)((DataPtr)[1]))<<16U))                    \
    | ((uint32)(((uint32)((DataPtr)[0]))<<24U)))))                          \
    )                                                                       \

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Perform null pointer checks of the input parameters of the Protect function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr  Pointer to port/data communication state.
 * \param[in] DataPtr   Pointer to Data to be protected.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04ProtectNullPtrChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);

/**
 * \brief Perform input parameter checks of the Config parameter
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04ConfigParamChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
);


/**
 * \brief Perform null pointer checks of the input parameters of the Check function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr  Pointer to port/data communication state.
 * \param[in] DataPtr   Pointer to Data to be protected.
 * \param[in] Length    Length of data in Bytes.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04CheckNullPtrChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
);

/**
 * \brief Calculate the Crc value
 *
 * \param[in] DataPtr        Pointer to Data to be protected.
 * \param[in] CrcByteOffset  Offset of CRC value in bytes containing the most significant
 *                           byte of the CRC value (i.e. CrcByteOffset+3 contains the least
 *                           significant byte of the 4-byte CRC value).
 * \param[in] Length         Length of Data.
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
static FUNC(uint32, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length
);

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile P04.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Length    Length of Data.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04Protect
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
);

/**
 * \brief Check the received Data using the E2E Profile P04.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StatePtr  Pointer to port/data communication state.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Length    Length of Data.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04Check
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

FUNC(Std_ReturnType, E2E_CODE) E2E_P04Protect
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function
     * Note: Since State contains only a Counter where all values are valid, no
     *       check of State parameters is required. */
    if (E2E_EB_P04ProtectNullPtrChecks(ConfigPtr, StatePtr, DataPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P04ConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (Length == 0U)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (((uint16)(ConfigPtr->MinDataLength / 8U) > Length) ||
            ((uint16)(ConfigPtr->MaxDataLength / 8U) < Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((uint16)((ConfigPtr->Offset / 8U) + (E2EP04_EB_E2EHEADER_SIZE_BITS / 8U)) > Length)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute protect function */
        RetVal = E2E_EB_P04Protect(ConfigPtr, StatePtr, DataPtr, Length);
    }

    return RetVal;
}


FUNC(Std_ReturnType, E2E_CODE) E2E_P04Check
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function. Checking the state parameters
     * (Counter, Status) makes no sense since all values are allowed and Status
     * value is not read. */
    /* Note: Null pointer is allowed for DataPtr if Length equals 0.
     *       This may happen if queued communication is used and no data is available. */
    if (E2E_EB_P04CheckNullPtrChecks(ConfigPtr, StatePtr, DataPtr, Length) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P04ConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((NULL_PTR != DataPtr) && (Length == 0U))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && (((uint16)(ConfigPtr->MinDataLength / 8U)) > Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && (((uint16)(ConfigPtr->MaxDataLength / 8U)) < Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((Length != 0U) && ((uint16)((ConfigPtr->Offset / 8U) + (E2EP04_EB_E2EHEADER_SIZE_BITS / 8U)) > Length))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute check function */
        RetVal = E2E_EB_P04Check(ConfigPtr, StatePtr, DataPtr, Length);
    }

    return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_P04ProtectInit
(
    P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    {
        StatePtr->Counter = 0U;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_P04CheckInit
(
    P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    {
        /* Counter is initialized to maximum value instead of 0. */
        /* See RfC 68745 for details. */
        StatePtr->Counter = E2EP04_EB_COUNTER_MAXVALUE;
        StatePtr->Status = E2E_P04STATUS_ERROR;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}

FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P04MapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_P04CheckStatusType Status
)
{
    E2E_PCheckStatusType RetVal;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
        case E2E_P04STATUS_OK: /* fall through */
        case E2E_P04STATUS_OKSOMELOST:
            RetVal = E2E_P_OK;
            break;
        case E2E_P04STATUS_ERROR:
            RetVal = E2E_P_ERROR;
            break;
        case E2E_P04STATUS_REPEATED:
            RetVal = E2E_P_REPEATED;
            break;
        case E2E_P04STATUS_NONEWDATA:
            RetVal = E2E_P_NONEWDATA;
            break;
        case E2E_P04STATUS_WRONGSEQUENCE:
            RetVal = E2E_P_WRONGSEQUENCE;
            break;
        default:
            RetVal = E2E_P_ERROR;
            break;
        }
    }
    else
    {
        RetVal = E2E_P_ERROR;
    }

    return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04ProtectNullPtrChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
)
{
    Std_ReturnType RetVal;

    /* null pointer checks */
    if (NULL_PTR == ConfigPtr)
    {
       RetVal = E_NOT_OK;
    }
    else if (NULL_PTR == StatePtr)
    {
       RetVal = E_NOT_OK;
    }
    else if (NULL_PTR == DataPtr)
    {
       RetVal = E_NOT_OK;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}


static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04ConfigParamChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal;

    /* sanity and input checks: E2E_P04ConfigType */
    /* ConfigPtr->MaxDeltaCounter requires no check since all values are valid */
    if (0U != (ConfigPtr->Offset % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->MinDataLength % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->MaxDataLength % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (E2EP04_EB_E2EHEADER_SIZE_BITS > ConfigPtr->MinDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (ConfigPtr->MinDataLength > ConfigPtr->MaxDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((uint16)(ConfigPtr->MaxDataLength - E2EP04_EB_E2EHEADER_SIZE_BITS) < ConfigPtr->Offset)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04CheckNullPtrChecks
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* null pointer checks */
    if (NULL_PTR == ConfigPtr)
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (NULL_PTR == StatePtr)
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else if ((NULL_PTR == DataPtr) && (0U != Length))
    {
       RetVal = E2E_E_INPUTERR_NULL;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(uint32, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length
)
{
    uint32 Crc;

    /* compute CRC over bytes that are before CRC. */
    /* SCrc_IsFirstCall is set to TRUE, thus parameter SCrc_StartValue32P4 will be ignored */
    Crc = SCrc_CalculateCRC32P4(&DataPtr[0],
                                (uint32)CrcByteOffset,
                                E2EP04_EB_CRC_STARTVALUE,
                                TRUE);

    if ((uint16)(CrcByteOffset + E2EP04_EB_E2EHEADER_CRC_SIZE) < Length)
    {
        /* Compute CRC over bytes that are after CRC (if any).*/
        Crc = SCrc_CalculateCRC32P4(&DataPtr[CrcByteOffset + E2EP04_EB_E2EHEADER_CRC_SIZE],
                   ((uint32)Length - CrcByteOffset) - E2EP04_EB_E2EHEADER_CRC_SIZE, Crc, FALSE);
    }

    return Crc;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04Protect
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    /* 1) Compute Offset in bytes and values written into data */
    const uint16 Offset = ConfigPtr->Offset / 8U;
    const uint16 Counter = StatePtr->Counter;
    const uint32 DataID = ConfigPtr->DataID;

    /* 2) Write Length in Big Endian order (first high byte) */
    E2EP04_EB_WRITE2BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_LENGTH_OFFSET], Length);

    /* 3) Write Counter in Big Endian order */
    E2EP04_EB_WRITE2BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_COUNTER_OFFSET], Counter);

    /* 4) Write 4-byte DataID in Big Endian order */
    E2EP04_EB_WRITE4BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_DATAID_OFFSET], DataID);

    {
        const uint16 CrcByteOffset = Offset + E2EP04_EB_E2EHEADER_CRC_OFFSET;

        /* 5) Compute CRC */
        const uint32 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length);

        /* 6) Write CRC */
        E2EP04_EB_WRITE4BYTE_VALUE(&DataPtr[CrcByteOffset], ComputedCRC);
    }

    /* 7) Increment Counter */
    StatePtr->Counter = (uint16)((StatePtr->Counter + 1U) & 0xFFFFU);

    return E2E_E_OK;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P04Check
(
    P2CONST(E2E_P04ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P04CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    /* If Length equals 0 then DataPtr equals NULL and no data was received in this cycle. */
    const boolean NewDataAvailable = (Length != 0U) ? TRUE : FALSE;

    if (TRUE == NewDataAvailable)
    {
        /* 1) Compute offset */
        const uint16 Offset = ConfigPtr->Offset / 8U;
        const uint16 CrcByteOffset = Offset + E2EP04_EB_E2EHEADER_CRC_OFFSET;

        /* 2) Read Length */
        const uint16 ReceivedLength =
                E2EP04_EB_READ2BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_LENGTH_OFFSET]);

        /* 3) Read Counter */
        const uint16 ReceivedCounter =
                E2EP04_EB_READ2BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_COUNTER_OFFSET]);

        /* 4) Read DataID */
        const uint32 ReceivedDataID =
                E2EP04_EB_READ4BYTE_VALUE(&DataPtr[Offset+E2EP04_EB_E2EHEADER_DATAID_OFFSET]);

        /* 5) Read CRC */
        const uint32 ReceivedCRC =
                E2EP04_EB_READ4BYTE_VALUE(&DataPtr[CrcByteOffset]);

        /* 6) Compute CRC */
        const uint32 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length);

        /* 7) Do checks */
        if (ReceivedCRC != ComputedCRC)
        {
            StatePtr->Status = E2E_P04STATUS_ERROR;
        }
        else if (ReceivedDataID != ConfigPtr->DataID)
        {
            StatePtr->Status = E2E_P04STATUS_ERROR;
        }
        else if (ReceivedLength != Length)
        {
            StatePtr->Status = E2E_P04STATUS_ERROR;
        }
        else
        {
            /* correct CRC and E2E header content => check sequence counter */
            const uint16 DeltaCounter = (ReceivedCounter >= StatePtr->Counter) ?
                 (uint16)(ReceivedCounter - StatePtr->Counter) :
                 (uint16)((uint16)(E2EP04_EB_COUNTER_MAXVALUE - StatePtr->Counter)
                       + ReceivedCounter + 1U);

            if (DeltaCounter > ConfigPtr->MaxDeltaCounter)
            {
                /* counter too big, i.e. too many data in the sequence
                 * have been probably lost since the last reception. */
                StatePtr->Status = E2E_P04STATUS_WRONGSEQUENCE;
            }
            else if (1U < DeltaCounter)
            {
                /* the Counter is incremented by DeltaCounter
                 * (1 <= DeltaCounter <= MaxDeltaCounter), i.e.
                 * some Data in the sequence have been probably lost since
                 * the last correct reception, but this is within
                 * the configured tolerance range. */
                StatePtr->Status = E2E_P04STATUS_OKSOMELOST;
            }
            else if (1U == DeltaCounter)
            {
                /* the Counter is incremented by 1, i.e. no Data has
                 * been lost since the last correct data reception. */
                StatePtr->Status = E2E_P04STATUS_OK;
            }
            else /* DeltaCounter == 0U */
            {
                /* the counter is identical, i.e. data has already been
                 * received (repeated message) */
                StatePtr->Status = E2E_P04STATUS_REPEATED;
            }

            /* Store counter value for next received message. */
            StatePtr->Counter = ReceivedCounter;
        }
    }
    else
    {
        /* no new data available since the last call */
        StatePtr->Status = E2E_P04STATUS_NONEWDATA;
    }

    return E2E_E_OK;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
