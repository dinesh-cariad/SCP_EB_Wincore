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

/*==================[inclusions]=============================================*/

#include <E2E_PJLR.h>       /* E2E Profile JLR         */
#include <SCrc_CRC8.h>      /* SCrc routine CRC8    */

/*==================[macros]=================================================*/

#if (defined E2EPJLR_EB_CRC_STARTVALUE) /* to prevent double declaration */
#error E2EPJLR_EB_CRC_STARTVALUE is already defined
#endif /* #if (defined E2EPJLR_EB_CRC_STARTVALUE) */

/** \brief Start value for Crc calculation */
#define E2EPJLR_EB_CRC_STARTVALUE 0x0U

#if (defined E2EPJLR_EB_CRC_LENGTH_BYTES) /* to prevent double declaration */
#error E2EPJLR_EB_CRC_LENGTH_BYTES is already defined
#endif /* #if (defined E2EPJLR_EB_CRC_LENGTH_BYTES) */

/** \brief Crc Length in Bytes */
#define E2EPJLR_EB_CRC_LENGTH_BYTES 0x1U

#if (defined E2EPJLR_EB_COUNTER_MAXVALUE) /* to prevent double declaration */
#error E2EPJLR_EB_COUNTER_MAXVALUE is already defined
#endif /* #if (defined E2EPJLR_EB_COUNTER_MAXVALUE) */

/** \brief Maximum value for state counter */
#define E2EPJLR_EB_COUNTER_MAXVALUE 0x0FU


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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRProtectNullPtrChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRConfigParamChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
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
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRCheckNullPtrChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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
static FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length,
    uint16 DataID,
    E2E_PJLRDataIDMode DataIDMode
);

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile PJLR.
 *
 * \param[in]     ConfigPtr          Pointer to static configuration.
 * \param[in,out] StatePtr           Pointer to port/data communication state counter variable.
 * \param[in,out] DataPtr            Pointer to Data to be protected.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRProtect
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) CounterPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);

/**
 * \brief Check the received Data using the E2E Profile PJLR.
 *
 * \param[in]     ConfigPtr Pointer to static configuration.
 * \param[in,out] StateCounterPtr  Pointer to port/data communication state counter value.
 * \param[in,out] DataPtr   Pointer to Data to be protected.
 *
 * \pre All input parameters are valid
 *
 * \return Profile JLR communication status value of the passed data
 */
static FUNC(E2E_PJLRCheckStatusType, E2E_CODE) E2E_EB_PJLRCheck
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) StateCounterPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRProtect
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function
     * Note: Since State contains only a Counter where all values are valid, no
     *       check of State parameters is required. */
    if (E2E_EB_PJLRProtectNullPtrChecks(ConfigPtr, StatePtr, DataPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_PJLRConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((uint16)(ConfigPtr->DataLength / 8U) != Length)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute protect function */
        RetVal = E2E_EB_PJLRProtect(
                ConfigPtr,
                &(StatePtr->Counter),
                DataPtr);
    }

    return RetVal;
}


FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRCheck
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 Length
)
{
    Std_ReturnType RetVal;

    /* First verify inputs of the protect function. Checking the state parameters
     * (Counter, Status) makes no sense since all values are allowed and Status
     * value is not read. */
    /* Note: Null pointer is allowed for DataPtr.
     *       This may happen if queued communication is used and no data is available. */
    if (E2E_EB_PJLRCheckNullPtrChecks(ConfigPtr, StatePtr, DataPtr, Length) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_PJLRConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((NULL_PTR != DataPtr) && (0U == Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((0U != Length) && (((uint16)(ConfigPtr->DataLength / 8U)) != Length))
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        /* input ok => execute check function */
        const E2E_PJLRCheckStatusType E2EPJLR_EB_Status = E2E_EB_PJLRCheck(
                ConfigPtr,
                &(StatePtr->Counter),
                DataPtr);

        /* map generic profile status to profile specific status value */
        switch (E2EPJLR_EB_Status)
        {
        case E2E_PJLRSTATUS_OK:
            StatePtr->Status = E2E_PJLRSTATUS_OK;
            break;
        case E2E_PJLRSTATUS_NONEWDATA:
            StatePtr->Status = E2E_PJLRSTATUS_NONEWDATA;
            break;
        case E2E_PJLRSTATUS_WRONGSEQUENCE:
            StatePtr->Status = E2E_PJLRSTATUS_WRONGSEQUENCE;
            break;
        case E2E_PJLRSTATUS_REPEATED:
            StatePtr->Status = E2E_PJLRSTATUS_REPEATED;
            break;
        case E2E_PJLRSTATUS_OKSOMELOST:
            StatePtr->Status = E2E_PJLRSTATUS_OKSOMELOST;
            break;
        default:
            StatePtr->Status = E2E_PJLRSTATUS_ERROR;
            break;
        }

        RetVal = E2E_E_OK;
    }

    return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRProtectInit
(
    P2VAR(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
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

FUNC(Std_ReturnType, E2E_CODE) E2E_PJLRCheckInit
(
    P2VAR(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if (NULL_PTR != StatePtr)
    {
        /* Counter is initialized to maximum value instead of 0. */
        /* See RfC 68745 for details. */
        StatePtr->Counter = E2EPJLR_EB_COUNTER_MAXVALUE;
        StatePtr->Status = E2E_PJLRSTATUS_ERROR;
        RetVal = E2E_E_OK;
    }
    else
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }

    return RetVal;
}

FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PJLRMapStatusToSM
(
    Std_ReturnType CheckReturn,
    E2E_PJLRCheckStatusType Status
)
{
    E2E_PCheckStatusType RetVal;

    if (CheckReturn == E2E_E_OK)
    {
        switch (Status)
        {
        case E2E_PJLRSTATUS_OK: /* fall through */
        case E2E_PJLRSTATUS_OKSOMELOST:
            RetVal = E2E_P_OK;
            break;
        case E2E_PJLRSTATUS_ERROR:
            RetVal = E2E_P_ERROR;
            break;
        case E2E_PJLRSTATUS_REPEATED:
            RetVal = E2E_P_REPEATED;
            break;
        case E2E_PJLRSTATUS_NONEWDATA:
            RetVal = E2E_P_NONEWDATA;
            break;
        case E2E_PJLRSTATUS_WRONGSEQUENCE:
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


static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRProtectNullPtrChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_PJLRProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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


static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRConfigParamChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal;

    /* sanity and input checks: E2E_PJLRConfigType */
    /* ConfigPtr->MaxDeltaCounter requires no check since all values are valid */
    if (0U != (ConfigPtr->CRCOffset % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((ConfigPtr->CRCOffset + 8U) > ConfigPtr->DataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->DataLength % 8U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((E2E_PJLR_DATAID_BOTH != ConfigPtr->DataIDMode) &&
             (E2E_PJLR_DATAID_NIBBLE != ConfigPtr->DataIDMode))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((E2E_PJLR_DATAID_NIBBLE == ConfigPtr->DataIDMode) &&
            (0x0FFFU < ConfigPtr->DataID))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (0U != (ConfigPtr->CounterOffset % 4U))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((ConfigPtr->CounterOffset + 4U) > ConfigPtr->DataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((E2E_PJLR_DATAID_NIBBLE == ConfigPtr->DataIDMode) && (0U != (ConfigPtr->DataIDNibbleOffset % 4U)))
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if ((ConfigPtr->DataIDNibbleOffset + 4U) > ConfigPtr->DataLength)
    {
       RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
       RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRCheckNullPtrChecks
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_PJLRCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
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

static FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr,
    uint16 CrcByteOffset,
    uint16 Length,
    uint16 DataID,
    E2E_PJLRDataIDMode DataIDMode
)
{
   const uint8 DataIDLow  = (uint8)(DataID & (uint16)0x00FFU);
   uint8 Crc;

   /* CRC over DataId (low byte) */
   Crc = SCrc_CalculateCRC8(&DataIDLow, E2EPJLR_EB_CRC_LENGTH_BYTES, E2EPJLR_EB_CRC_STARTVALUE, FALSE);

   /* CRC over DataId (high byte) */
   if(E2E_PJLR_DATAID_BOTH == DataIDMode)
   {
     const uint8 DataIDHigh = (uint8)((DataID >> 8U) & (uint16)0x00FFU);
     Crc = SCrc_CalculateCRC8(&DataIDHigh, E2EPJLR_EB_CRC_LENGTH_BYTES, Crc, FALSE);
   }
   else
   {
     const uint8 DataIDHigh = 0U;
     Crc = SCrc_CalculateCRC8(&DataIDHigh, E2EPJLR_EB_CRC_LENGTH_BYTES, Crc, FALSE);
   }

    /* compute CRC over data bytes */
    if(0U < CrcByteOffset)
    {
        Crc = SCrc_CalculateCRC8(&DataPtr[0U], CrcByteOffset, Crc, FALSE);

        if(Length > (CrcByteOffset + 1U))
        {
          Crc = SCrc_CalculateCRC8(&DataPtr[CrcByteOffset+1U], (((uint32)Length - CrcByteOffset) - 1U), Crc, FALSE);
        }
        else
        {
          /* To prevent wrong if-else association */
        }
    }
    else
    {
        Crc = SCrc_CalculateCRC8(&DataPtr[1U], ((uint32)Length - 1U), Crc, FALSE);
    }

    return Crc;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PJLRProtect
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) CounterPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
)
{

    uint16 Length = (ConfigPtr->DataLength / 8U);
    uint16 DataID = ConfigPtr->DataID;
    E2E_PJLRDataIDMode DataIDMode = ConfigPtr->DataIDMode;
    uint16 CRCOffset = ConfigPtr->CRCOffset;
    uint16 CounterOffset = ConfigPtr->CounterOffset;
    uint16 CounterByteOffset = (uint16)(ConfigPtr->CounterOffset / 8U);
    uint16 DataIDNibbleOffset = ConfigPtr->DataIDNibbleOffset;
    uint16 DataIDNibbleByteOffset = (uint16)(ConfigPtr->DataIDNibbleOffset / 8U);

    /* Compute values written into data */
    const uint8 Counter = *CounterPtr;

    /* 1) Write DataIDNibble */
    if(E2E_PJLR_DATAID_NIBBLE == DataIDMode)
    {
      const uint8 DataIDNibble = (uint8)((DataID & (uint16)0x0F00U) >> 8U);

      if ( 0U == (DataIDNibbleOffset % 8U) )
      {
        /* DataIDNibble is written to low nibble */
        DataPtr[DataIDNibbleByteOffset] = (uint8)(DataPtr[DataIDNibbleByteOffset] & 0xF0U);
        DataPtr[DataIDNibbleByteOffset] = (uint8)(DataPtr[DataIDNibbleByteOffset] | DataIDNibble);
      }
      else
      {
        /* DataIDNibble is written to high nibble */
        DataPtr[DataIDNibbleByteOffset] = (uint8)(DataPtr[DataIDNibbleByteOffset] & 0x0FU);
        DataPtr[DataIDNibbleByteOffset] = (uint8)((DataPtr[DataIDNibbleByteOffset] | (DataIDNibble << 4U)) & 0xFFU);
      }
    }

    /* 2) Write Counter */
    if ( 0U == (CounterOffset % 8U) )
    {
      /* Counter is written to low nibble */
      DataPtr[CounterByteOffset] = (uint8)(DataPtr[CounterByteOffset] & 0xF0U);
      DataPtr[CounterByteOffset] = (uint8)(DataPtr[CounterByteOffset] | Counter);
    }
    else
    {
      /* Counter is written to high nibble */
      DataPtr[CounterByteOffset] = (uint8)(DataPtr[CounterByteOffset] & 0x0FU);
      DataPtr[CounterByteOffset] = (uint8)((DataPtr[CounterByteOffset] | (Counter << 4U)) & 0xFFU);
    }

    {
        const uint16 CrcByteOffset = (uint16)(CRCOffset / 8U);

        /* 3) Compute CRC */
        const uint8 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length, DataID, DataIDMode);

        /* 4) Write CRC */
        DataPtr[CrcByteOffset] = ComputedCRC;
    }

    /* 5) Increment Counter */
   /* update sequence counter modulo 15 */
   if (E2EPJLR_EB_COUNTER_MAXVALUE > Counter)
   {
      (*CounterPtr) = (*CounterPtr) + 1U;
   }
   else
   {
      (*CounterPtr) = 0U;
   }

    return E2E_E_OK;
}

static FUNC(E2E_PJLRCheckStatusType, E2E_CODE) E2E_EB_PJLRCheck
(
    P2CONST(E2E_PJLRConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) StateCounterPtr,
    P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) DataPtr
)
{
    E2E_PJLRCheckStatusType RetVal = E2E_PJLRSTATUS_ERROR;
    /* If DataPtr equals NULL, then no data was received in this cycle. */
    const boolean NewDataAvailable = (DataPtr != NULL_PTR) ? TRUE : FALSE;

    if (TRUE == NewDataAvailable)
    {
        uint16 Length = (ConfigPtr->DataLength / 8U);
        uint16 DataID = ConfigPtr->DataID;
        uint8 MaxDeltaCounter = ConfigPtr->MaxDeltaCounter;
        E2E_PJLRDataIDMode DataIDMode = ConfigPtr->DataIDMode;
        uint16 CRCOffset = ConfigPtr->CRCOffset;
        uint16 CounterOffset = ConfigPtr->CounterOffset;
        uint16 DataIDNibbleOffset = ConfigPtr->DataIDNibbleOffset;

        const uint8 LastStateCounter = *StateCounterPtr;

       /* Read DataIDNibble value which is placed at different positions depending on tx of Length */
        const uint8 DataIDNibbleByte = DataPtr[(uint16)(DataIDNibbleOffset / 8U)];
        const uint8 ReceivedNibble = (0U == (DataIDNibbleOffset % 8U)) ?
                                     (uint8)(DataIDNibbleByte & 0x0FU) : (uint8)((DataIDNibbleByte & 0xF0U) >> 4U);

        /* Read Counter value which is placed at different positions depending on tx of Length */
        const uint8 CounterNibbleByte = DataPtr[(uint16)(CounterOffset / 8U)];
        const uint8 ReceivedCounter = (0U == (CounterOffset % 8U)) ?
                                     (uint8)(CounterNibbleByte & 0x0FU) : (uint8)((CounterNibbleByte & 0xF0U) >> 4U);

        /* Read CRC */
        const uint16 CrcByteOffset = (uint16)(CRCOffset / 8U);
        const uint8 ReceivedCRC = DataPtr[CrcByteOffset];

        /* Compute CRC */
        const uint8 ComputedCRC = E2E_EB_CalculateCrc(DataPtr, CrcByteOffset, Length, DataID, DataIDMode);

        /* Do checks */
        if (ReceivedCRC != ComputedCRC)
        {
            RetVal = E2E_PJLRSTATUS_ERROR;
        }
        else if ( (E2E_PJLR_DATAID_NIBBLE == DataIDMode) && (ReceivedNibble != ((uint8)((DataID >> 8U) & (uint16)0x0FU))) )
        {
            RetVal = E2E_PJLRSTATUS_ERROR;
        }
        else
        {
            /* correct CRC and E2E header content => check sequence counter */
            const uint8 DeltaCounter = (ReceivedCounter >= LastStateCounter) ?
                  (uint8)(ReceivedCounter - LastStateCounter) :
                  (uint8)((E2EPJLR_EB_COUNTER_MAXVALUE +  ReceivedCounter + 1U )
                      - LastStateCounter);

            if (DeltaCounter > MaxDeltaCounter)
            {
                /* counter too big, i.e. too many data in the sequence
                 * have been probably lost since the last reception. */
                RetVal = E2E_PJLRSTATUS_WRONGSEQUENCE;
            }
            else if (1U < DeltaCounter)
            {
                /* the Counter is incremented by DeltaCounter
                 * (1 <= DeltaCounter <= MaxDeltaCounter), i.e.
                 * some Data in the sequence have been probably lost since
                 * the last correct reception, but this is within
                 * the configured tolerance range. */
                RetVal = E2E_PJLRSTATUS_OKSOMELOST;
            }
            else if (1U == DeltaCounter)
            {
                /* the Counter is incremented by 1, i.e. no Data has
                 * been lost since the last correct data reception. */
                RetVal = E2E_PJLRSTATUS_OK;
            }
            else /* DeltaCounter == 0U */
            {
                /* the counter is identical, i.e. data has already been
                 * received (repeated message) */
                RetVal = E2E_PJLRSTATUS_REPEATED;
            }

            /* Store counter value for next received message. */
            *StateCounterPtr = ReceivedCounter;
        }
    }
    else
    {
        /* no new data available since the last call */
        RetVal = E2E_PJLRSTATUS_NONEWDATA;
    }

    return RetVal;
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
