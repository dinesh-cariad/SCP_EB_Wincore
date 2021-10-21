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

/*==================[inclusions]=============================================*/

#include <E2E_P06.h>       /* E2E Profile 6         */
#include <SCrc_CRC16.h>    /* SCrc routine CRC16    */

/*==================[macros]=================================================*/

#if (defined E2EP06_EB_CRC_STARTVALUE) /* to prevent double declaration */
#error E2EP06_EB_CRC_STARTVALUE is already defined
#endif /* #if (defined E2EP06_EB_CRC_STARTVALUE) */

/** \brief Start value for Crc calculation */
#define E2EP06_EB_CRC_STARTVALUE 0xFFFFU

#if (defined E2EP06_EB_COUNTER_MAXVALUE) /* to prevent double declaration */
#error E2EP06_EB_COUNTER_MAXVALUE is already defined
#endif /* #if (defined E2EP06_EB_COUNTER_MAXVALUE) */

/** \brief Maximum value for state counter */
#define E2EP06_EB_COUNTER_MAXVALUE 0xFFU

#if (defined E2EP06_EB_E2EHEADER_CRC_OFFSET) /* to prevent double declaration */
#error E2EP06_EB_E2EHEADER_CRC_OFFSET is already defined
#endif /* #if (defined E2EP06_EB_E2EHEADER_CRC_OFFSET) */

/** \brief Offset in bytes within E2E header where most significant byte of CRC is coded */
#define E2EP06_EB_E2EHEADER_CRC_OFFSET 0U

#if (defined E2EP06_EB_E2EHEADER_CRC_SIZE) /* to prevent double declaration */
#error E2EP06_EB_E2EHEADER_CRC_SIZE is already defined
#endif /* #if (defined E2EP06_EB_E2EHEADER_CRC_SIZE) */

/** \brief Crc size in bytes within E2E header. */
#define E2EP06_EB_E2EHEADER_CRC_SIZE 2U

#if (defined E2EP06_EB_E2EHEADER_LENGTH_OFFSET) /* to prevent double declaration */
#error E2EP06_EB_E2EHEADER_LENGTH_OFFSET is already defined
#endif /* #if (defined E2EP06_EB_E2EHEADER_LENGTH_OFFSET) */

/** \brief Offset in bytes within E2E header where most significant byte of Length is coded */
#define E2EP06_EB_E2EHEADER_LENGTH_OFFSET 2U

#if (defined E2EP06_EB_E2EHEADER_COUNTER_OFFSET) /* to prevent double declaration */
#error E2EP06_EB_E2EHEADER_COUNTER_OFFSET is already defined
#endif /* #if (defined E2EP06_EB_E2EHEADER_COUNTER_OFFSET) */

/** \brief Offset in bytes within E2E header where most significant byte of Counter is coded */
#define E2EP06_EB_E2EHEADER_COUNTER_OFFSET 4U

#if (defined E2EP06_EB_E2EHEADER_SIZE_BITS) /* to prevent double declaration */
#error E2EP06_EB_E2EHEADER_SIZE_BITS is already defined
#endif /* #if (defined E2EP06_EB_E2EHEADER_SIZE_BITS) */

/** \brief E2E Profile 06 header size in bits. */
#define E2EP06_EB_E2EHEADER_SIZE_BITS 40U

#if (defined E2EP06_EB_WRITE2BYTE_VALUE) /* to prevent double declaration */
#error E2EP06_EB_WRITE2BYTE_VALUE is already defined
#endif /* #if (defined E2EP06_EB_WRITE2BYTE_VALUE) */

/** \brief Write a 2-byte value in Big Endian order to the passed Data Pointer */
#define E2EP06_EB_WRITE2BYTE_VALUE(DataPtr, Value)                       \
do {                                                                     \
    (DataPtr)[0] = (uint8)((uint16)((Value) >> 8U) & (uint8)0xFFU);      \
    (DataPtr)[1] = (uint8)((Value) & (uint8)0xFFU);                      \
} while (0)


#if (defined E2EP06_EB_READ2BYTE_VALUE) /* to prevent double declaration */
#error E2EP06_EB_READ2BYTE_VALUE is already defined
#endif /* #if (defined E2EP06_EB_READ2BYTE_VALUE) */

/** \brief Read a 2-byte value in Big Endian order from the passed Data Pointer */
#define E2EP06_EB_READ2BYTE_VALUE(DataPtr)                               \
    (                                                                    \
      (uint16)(((uint16)((DataPtr)[1]))                                  \
    | ((uint16)(((uint16)((DataPtr)[0]))<<8U)))                          \
    )                                                                    \

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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06ProtectNullPtrChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06ConfigParamChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06CheckNullPtrChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
 * \param[in] DataID         DataID used in CRC calculation.
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
static FUNC(uint16, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length,
    uint16 DataID
);

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile P06.
 *
 * \param[in,out] StatePtr  Pointer to port/data communication state counter variable.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Offset    Byte offset of E2E header.
 * \param[in]     DataID    DataID used for CRC calculation.
 * \param[in]     Length    Length of Data.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06Protect
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) CounterPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Offset,
    uint16 DataID,
    uint16 Length
);

/**
 * \brief Check the received Data using the E2E Profile P06.
 *
 * \param[in,out] StateCounterPtr  Pointer to port/data communication state counter value.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 * \param[in]     Offset    Byte offset of E2E header of static configuration structure.
 * \param[in]     DataID    DataID used for CRC calculation of static configuration structure.
 * \param[in]     Length    Length of Data.
 * \param[in]     MaxDeltaCounter MaxDeltaCounter init value of static configuration structure.
 *
 * \pre All input parameters are valid
 *
 * \return and Profile 6 communication status value of the passed data
 */
static FUNC(E2E_P06CheckStatusType, E2E_CODE) E2E_EB_P06Check
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) StateCounterPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Offset,
    uint16 DataID,
    uint16 Length,
    uint8 MaxDeltaCounter
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

FUNC(Std_ReturnType, E2E_CODE) E2E_P06Protect
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function
     * Note: Since State contains only a Counter where all values are valid, no
     *       check of State parameters is required. */
    if (E2E_EB_P06ProtectNullPtrChecks(ConfigPtr, StatePtr, DataPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P06ConfigParamChecks(ConfigPtr) != E_OK)
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
    else if ((uint16)((ConfigPtr->Offset / 8U) + (E2EP06_EB_E2EHEADER_SIZE_BITS / 8U)) > Length)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute protect function */
        RetVal = E2E_EB_P06Protect(
                &(StatePtr->Counter),
                DataPtr,
                ConfigPtr->Offset / 8U,
                ConfigPtr->DataID,
                Length);
    }

    return RetVal;
}


FUNC(Std_ReturnType, E2E_CODE) E2E_P06Check
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
    if (E2E_EB_P06CheckNullPtrChecks(ConfigPtr, StatePtr, DataPtr, Length) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_P06ConfigParamChecks(ConfigPtr) != E_OK)
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
    else if ((Length != 0U) && ((uint16)((ConfigPtr->Offset / 8U) + (E2EP06_EB_E2EHEADER_SIZE_BITS / 8U)) > Length))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute check function */
        const E2E_P06CheckStatusType E2EP06_EB_Status = E2E_EB_P06Check(
                &(StatePtr->Counter),
                DataPtr,
                ConfigPtr->Offset / 8U,
                ConfigPtr->DataID,
                Length,
                ConfigPtr->MaxDeltaCounter);

        /* map generic profile status to profile specific status value */
        switch (E2EP06_EB_Status)
        {
        case E2E_P06STATUS_OK:
            StatePtr->Status = E2E_P06STATUS_OK;
            break;
        case E2E_P06STATUS_NONEWDATA:
            StatePtr->Status = E2E_P06STATUS_NONEWDATA;
            break;
        case E2E_P06STATUS_WRONGSEQUENCE:
            StatePtr->Status = E2E_P06STATUS_WRONGSEQUENCE;
            break;
        case E2E_P06STATUS_REPEATED:
            StatePtr->Status = E2E_P06STATUS_REPEATED;
            break;
        case E2E_P06STATUS_OKSOMELOST:
            StatePtr->Status = E2E_P06STATUS_OKSOMELOST;
            break;
        default:
            StatePtr->Status = E2E_P06STATUS_ERROR;
            break;
        }

        RetVal = E2E_E_OK;
    }

    return RetVal;
}


FUNC(Std_ReturnType, E2E_CODE) E2E_P06ProtectInit
(
    P2VAR(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
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

FUNC(Std_ReturnType, E2E_CODE) E2E_P06CheckInit
(
    P2VAR(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    {
        /* Counter is initialized to maximum value instead of 0. */
        /* See RfC 68745 for details. */
        StatePtr->Counter = E2EP06_EB_COUNTER_MAXVALUE;
        StatePtr->Status = E2E_P06STATUS_ERROR;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}

FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_P06MapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_P06CheckStatusType Status
)
{
    E2E_PCheckStatusType RetVal;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
        case E2E_P06STATUS_OK: /* fall through */
        case E2E_P06STATUS_OKSOMELOST:
            RetVal = E2E_P_OK;
            break;
        case E2E_P06STATUS_ERROR:
            RetVal = E2E_P_ERROR;
            break;
        case E2E_P06STATUS_REPEATED:
            RetVal = E2E_P_REPEATED;
            break;
        case E2E_P06STATUS_NONEWDATA:
            RetVal = E2E_P_NONEWDATA;
            break;
        case E2E_P06STATUS_WRONGSEQUENCE:
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

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06ProtectNullPtrChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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


static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06ConfigParamChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal;

    /* sanity and input checks: E2E_P06ConfigType */
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
    else if (E2EP06_EB_E2EHEADER_SIZE_BITS > ConfigPtr->MinDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (ConfigPtr->MinDataLength > ConfigPtr->MaxDataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06CheckNullPtrChecks
(
    P2CONST(E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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

static FUNC(uint16, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length,
    uint16 DataID
)
{
    const uint8 DataIDHigh = (uint8)(DataID >> 8U);
    const uint8 DataIDLow  = (uint8)(DataID & (uint8)0xFFU);
    uint16 Crc;

    /* compute CRC over data bytes */
    if (0U < CrcByteOffset)
    {
       Crc = SCrc_CalculateCRC16(&DataPtr[0], CrcByteOffset, E2EP06_EB_CRC_STARTVALUE, TRUE);
       Crc = SCrc_CalculateCRC16
             (
                 &DataPtr[CrcByteOffset + E2EP06_EB_E2EHEADER_CRC_SIZE],
                 ((uint32) Length - CrcByteOffset) - E2EP06_EB_E2EHEADER_CRC_SIZE,
                 Crc,
                 FALSE
             );
    }
    else
    {
      Crc = SCrc_CalculateCRC16
            (
                &DataPtr[E2EP06_EB_E2EHEADER_CRC_SIZE],
                (uint32) Length - E2EP06_EB_E2EHEADER_CRC_SIZE,
                E2EP06_EB_CRC_STARTVALUE,
                TRUE
            );
    }

    /* CRC over DataId (high byte) */
    Crc = SCrc_CalculateCRC16(&DataIDHigh, 1U, Crc, FALSE);
    /* CRC over DataId (low byte) */
    Crc = SCrc_CalculateCRC16(&DataIDLow, 1U, Crc, FALSE);

    return Crc;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P06Protect
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) CounterPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Offset,
    uint16 DataID,
    uint16 Length
)
{
    /* 1) Compute values written into data */
    const uint8 Counter = *CounterPtr;

    /* 2) Write Length in Big Endian order (first high byte) */
    E2EP06_EB_WRITE2BYTE_VALUE
    (
        &DataPtr[(uint16)(Offset + E2EP06_EB_E2EHEADER_LENGTH_OFFSET)],
        Length
    );

    /* 3) Write Counter in Big Endian order */
    DataPtr[(uint16)(Offset + E2EP06_EB_E2EHEADER_COUNTER_OFFSET)] = Counter;

    {
        const uint16 CrcByteOffset = Offset + E2EP06_EB_E2EHEADER_CRC_OFFSET;

        /* 4) Compute CRC */
        const uint16 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length, DataID);

        /* 5) Write CRC */
        E2EP06_EB_WRITE2BYTE_VALUE(&DataPtr[CrcByteOffset], ComputedCRC);
    }

    /* 6) Increment Counter */
    (*CounterPtr) = (uint8)(((*CounterPtr) + 1U) & 0xFFU);

    return E2E_E_OK;
}

static FUNC(E2E_P06CheckStatusType, E2E_CODE) E2E_EB_P06Check
(
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) StateCounterPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Offset,
    uint16 DataID,
    uint16 Length,
    uint8 MaxDeltaCounter
)
{
    E2E_P06CheckStatusType RetVal = E2E_P06STATUS_ERROR;
    /* If DataPtr equals NULL, then no data was received in this cycle. */
    const boolean NewDataAvailable = (DataPtr != NULL_PTR) ? TRUE : FALSE;

    if (TRUE == NewDataAvailable)
    {
        const uint8 LastStateCounter = *StateCounterPtr;
        /* Compute offsets */
        const uint16 CrcByteOffset = Offset + E2EP06_EB_E2EHEADER_CRC_OFFSET;

        /* Read Counter value which is placed at different positions depending on tx of Length */
        const uint8 ReceivedCounter = DataPtr[Offset+E2EP06_EB_E2EHEADER_COUNTER_OFFSET];

        /* Read Length value */
        const uint16 ReceivedLength =
          E2EP06_EB_READ2BYTE_VALUE(&DataPtr[(uint16)(Offset + E2EP06_EB_E2EHEADER_LENGTH_OFFSET)]);

        /* Read CRC */
        const uint16 ReceivedCRC = E2EP06_EB_READ2BYTE_VALUE(&DataPtr[CrcByteOffset]);

        /* Compute CRC */
        const uint16 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length, DataID);

        /* Do checks */
        if (ReceivedCRC != ComputedCRC)
        {
            RetVal = E2E_P06STATUS_ERROR;
        }
        else if (ReceivedLength != Length)
        {
            RetVal = E2E_P06STATUS_ERROR;
        }
        else
        {
            /* correct CRC and E2E header content => check sequence counter */
            const uint8 DeltaCounter = (ReceivedCounter >= LastStateCounter) ?
                  (uint8)(ReceivedCounter - LastStateCounter) :
                  (uint8)((uint8)(E2EP06_EB_COUNTER_MAXVALUE - LastStateCounter)
                        +  ReceivedCounter + 1U);

            if (DeltaCounter > MaxDeltaCounter)
            {
                /* counter too big, i.e. too many data in the sequence
                 * have been probably lost since the last reception. */
                RetVal = E2E_P06STATUS_WRONGSEQUENCE;
            }
            else if (1U < DeltaCounter)
            {
                /* the Counter is incremented by DeltaCounter
                 * (1 <= DeltaCounter <= MaxDeltaCounter), i.e.
                 * some Data in the sequence have been probably lost since
                 * the last correct reception, but this is within
                 * the configured tolerance range. */
                RetVal = E2E_P06STATUS_OKSOMELOST;
            }
            else if (1U == DeltaCounter)
            {
                /* the Counter is incremented by 1, i.e. no Data has
                 * been lost since the last correct data reception. */
                RetVal = E2E_P06STATUS_OK;
            }
            else /* DeltaCounter == 0U */
            {
                /* the counter is identical, i.e. data has already been
                 * received (repeated message) */
                RetVal = E2E_P06STATUS_REPEATED;
            }

            /* Store counter value for next received message. */
            *StateCounterPtr = ReceivedCounter;
        }
    }
    else
    {
        /* no new data available since the last call */
        RetVal = E2E_P06STATUS_NONEWDATA;
    }

    return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
