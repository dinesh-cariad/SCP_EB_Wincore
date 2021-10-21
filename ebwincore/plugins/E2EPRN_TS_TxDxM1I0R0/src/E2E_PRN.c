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

/*==================[inclusions]=============================================*/

#include <E2E_PRN.h>      /* E2E Profile PRN      */
#include <SCrc_CRC8.h>    /* SCrc routine CRC8  */

/*==================[macros]=================================================*/

#if (defined E2EPRN_CRC_STARTVALUE) /* to prevent double declaration */
#error E2EPRN_CRC_STARTVALUE is already defined
#endif /* #if (defined E2EPRN_CRC_STARTVALUE) */

/** \brief Start value for Crc calculation
 *
 * Note: The CalculateCRC8() API is XORing this start value with 0x00
 * resulting with actual internal start value equal to 0xFF.
 */
#define E2EPRN_CRC_STARTVALUE 0x00U

#if (defined E2EPRN_CRC_FINALXORVALUE) /* to prevent double declaration */
#error E2EPRN_CRC_FINALXORVALUE is already defined
#endif /* #if (defined E2EPRN_CRC_FINALXORVALUE) */

/** \brief XOR value which is applied on the result before returned by the Crc routine */
#define E2EPRN_CRC_FINALXORVALUE 0x00U

#if (defined E2EPRN_COUNTER_MAX) /* to prevent double declaration */
#error E2EPRN_COUNTER_MAX is already defined
#endif /* #if (defined E2EPRN_COUNTER_MAX) */

/** \brief Maximum allowed counter value for E2E Profile PRN */
#define E2EPRN_COUNTER_MAX 15U    /* ASCE2E-629:Extend the Counter range [0..15]*/

#if (defined E2EPRN_DATALENGTH_MAX) /* to prevent double declaration */
#error E2EPRN_DATALENGTH_MAX is already defined
#endif /* #if (defined E2EPRN_DATALENGTH_MAX) */

/** \brief Maximum allowed data length for E2E Profile PRN in bits */
#define E2EPRN_DATALENGTH_MAX 2048U

#if (defined E2EPRN_DATALENGTH_MIN) /* to prevent double declaration */
#error E2EPRN_DATALENGTH_MIN is already defined
#endif /* #if (defined E2EPRN_DATALENGTH_MIN) */

/** \brief Minimum allowed data length for E2E Profile PRN in bits
 * shall be a multiple of 8 */
#define E2EPRN_DATALENGTH_MIN 16U

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Perform null pointer checks of the input parameters of the Protect function
 *
 * \param[in] Config Pointer to static configuration.
 * \param[in] State  Pointer to port/data communication state.
 * \param[in] Data   Pointer to Data to be protected.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectNullPtrChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);

/**
 * \brief Perform input parameter checks of the Config parameter of the Protect function
 *
 * \param[in] Config Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectConfigParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
);

/**
 * \brief Perform input parameter checks required by AUTOSAR of the Protect function
 *
 * \param[in] Config Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectASRParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
);

/**
 * \brief Perform input parameter checks of the State parameter of the Protect function
 *
 * \param[in] State  Pointer to port/data communication state.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectStateParamChecks
(
   P2CONST(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State
);

/**
 * \brief Perform null pointer checks of the input parameters of the Check function
 *
 * \param[in] Config Pointer to static configuration.
 * \param[in] State  Pointer to port/data communication state.
 * \param[in] Data   Pointer to Data to be protected.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckNullPtrChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);

/**
 * \brief Perform input parameter checks of the Config parameter of the Check function
 *
 * \param[in] Config Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckConfigParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
);

/**
 * \brief Perform input parameter checks required by AUTOSAR of the Check function
 *
 * \param[in] Config Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckASRParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
);

/**
 * \brief Perform input parameter checks of the State parameter of the Check function
 *
 * \param[in] State  Pointer to port/data communication state.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckStateParamChecks
(
   P2CONST(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State
);

/**
 * \brief Calculate the Crc value
 *
 * \param[in] Config        Pointer to static configuration.
 * \param[in] Data          Pointer to Data to be protected.
 * \param[in] CrcByteOffset Byte-offset of the Crc value
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
static FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data,
   uint8 CrcByteOffset
);

/**
 * \brief Protects the array/buffer to be transmitted using the E2E Profile PRN.
 *
 * \param[in]     Config Pointer to static configuration.
 * \param[in,out] State  Pointer to port/data communication state.
 * \param[in,out] Data   Pointer to Data to be protected.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtect
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);

/**
 * \brief Check the received Data using the E2E Profile PRN.
 *
 * \param[in]     Config Pointer to static configuration.
 * \param[in,out] State  Pointer to port/data communication state.
 * \param[in,out] Data   Pointer to Data to be protected.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_INPUTERR_NULL At least one pointer parameter is a NULL pointer.
 * \retval E2E_E_INPUTERR_WRONG At least one input parameter is erroneous.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheck
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);

/**
 * \brief Check the received Counter value using the E2E Profile PRN.
 *
 * \param[in]     Config Pointer to static configuration.
 * \param[in,out] State  Pointer to port/data communication state.
 * \param[in]     ReceivedCounter Received counter value.
 *
 * \pre All input parameters are valid
 * \post State variables are updated based on E2E Profile algorithm
 */
static FUNC(void, E2E_CODE) E2E_EB_PRNCheckCounter
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   uint8 ReceivedCounter
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

FUNC(Std_ReturnType, E2E_CODE) E2E_PRNProtect
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   Std_ReturnType RetVal;

   if (E2E_EB_PRNProtectNullPtrChecks(Config, State, Data) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_NULL;
   }
   else if (E2E_EB_PRNProtectConfigParamChecks(Config) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else if (E2E_EB_PRNProtectASRParamChecks(Config) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else if (E2E_EB_PRNProtectStateParamChecks(State) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else
   {
      RetVal = E2E_EB_PRNProtect(Config, State, Data);
   }

   return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_PRNCheck
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   Std_ReturnType RetVal;

   if (E2E_EB_PRNCheckNullPtrChecks(Config, State, Data) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_NULL;
   }
   else if (E2E_EB_PRNCheckConfigParamChecks(Config) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else if (E2E_EB_PRNCheckASRParamChecks(Config) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else if (E2E_EB_PRNCheckStateParamChecks(State) != E_OK)
   {
      RetVal = E2E_E_INPUTERR_WRONG;
   }
   else
   {
      RetVal = E2E_EB_PRNCheck(Config, State, Data);
   }

   return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_PRNProtectInit
(
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
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

FUNC(Std_ReturnType, E2E_CODE) E2E_PRNCheckInit
(
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
   Std_ReturnType RetVal;

   if (NULL_PTR != StatePtr)
   {
      StatePtr->LastValidCounter = 0U;
      StatePtr->MaxDeltaCounter = 0U;
      StatePtr->WaitForFirstData = FALSE;
      StatePtr->NewDataAvailable = TRUE;
      StatePtr->LostData = 0U;
      StatePtr->Status = E2E_PRNSTATUS_NONEWDATA;
      StatePtr->NoNewOrRepeatedDataCounter = 0U;
      StatePtr->SyncCounter = 0U;
      RetVal = E2E_E_OK;
   }
   else
   {
      RetVal = E2E_E_INPUTERR_NULL;
   }

   return RetVal;
}

FUNC(E2E_PCheckStatusType, E2E_CODE) E2E_PRNMapStatusToSM
(
   Std_ReturnType CheckReturn,
   E2E_PRNCheckStatusType Status,
   boolean profileBehavior
)
{
   E2E_PCheckStatusType RetVal;

   if (CheckReturn == E2E_E_OK)
   {
     if (profileBehavior == FALSE)
     {
        switch (Status)
        {
        case E2E_PRNSTATUS_OK: /* fall through */
        case E2E_PRNSTATUS_OKSOMELOST: /* fall through */
        case E2E_PRNSTATUS_INITIAL:
           RetVal = E2E_P_OK;
           break;
        case E2E_PRNSTATUS_WRONGCRC:
           RetVal = E2E_P_ERROR;
           break;
        case E2E_PRNSTATUS_REPEATED:
           RetVal = E2E_P_REPEATED;
           break;
        case E2E_PRNSTATUS_NONEWDATA:
           RetVal = E2E_P_NONEWDATA;
           break;
        case E2E_PRNSTATUS_WRONGSEQUENCE: /* fall through */
        case E2E_PRNSTATUS_SYNC:
           RetVal = E2E_P_WRONGSEQUENCE;
           break;
        default:
           RetVal = E2E_P_ERROR;
           break;
        }
     }
     else
     {
        switch (Status)
        {
        case E2E_PRNSTATUS_OK: /* fall through */
        case E2E_PRNSTATUS_OKSOMELOST: /* fall through */
        case E2E_PRNSTATUS_SYNC:
           RetVal = E2E_P_OK;
           break;
        case E2E_PRNSTATUS_WRONGCRC:
           RetVal = E2E_P_ERROR;
           break;
        case E2E_PRNSTATUS_REPEATED:
           RetVal = E2E_P_REPEATED;
           break;
        case E2E_PRNSTATUS_NONEWDATA:
           RetVal = E2E_P_NONEWDATA;
           break;
        case E2E_PRNSTATUS_WRONGSEQUENCE: /* fall through */
        case E2E_PRNSTATUS_INITIAL:
           RetVal = E2E_P_WRONGSEQUENCE;
           break;
        default:
           RetVal = E2E_P_ERROR;
           break;
        }
     }
   }
   else
   {
      RetVal = E2E_P_ERROR;
   }

   return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectNullPtrChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   Std_ReturnType RetVal;

   /* null pointer checks */
   if (NULL_PTR == Config)
   {
      RetVal = E_NOT_OK;
   }
   else if (NULL_PTR == State)
   {
      RetVal = E_NOT_OK;
   }
   else if (NULL_PTR == Data)
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectConfigParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
)
{
   Std_ReturnType RetVal;

   /* sanity and input checks: E2E_PRNConfigType */
   /* Config->MaxDeltaCounterInit requires no check since it is not read */
   /* Config->MaxNoNewOrRepeatedData requires no check since it is not read */
   /* Config->SyncCounterInit requires no check since it is not read */
   if (E2EPRN_DATALENGTH_MAX < Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_DATALENGTH_MIN > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (0U != (Config->DataLength % 8U))
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->CounterOffset + 4U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->DataIDNibbleOffset + 4U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->CRCOffset + 8U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (((E2E_PRN_DATAID_BOTH != Config->DataIDMode) &&
             (E2E_PRN_DATAID_LOW != Config->DataIDMode)) &&
             (E2E_PRN_DATAID_NIBBLE != Config->DataIDMode))
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectASRParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
)
{
   Std_ReturnType RetVal;

   if ((E2E_PRN_DATAID_LOW == Config->DataIDMode) &&
            (0x00FFU < Config->DataID))
   {
      RetVal = E_NOT_OK;
   }
   else if ((E2E_PRN_DATAID_NIBBLE == Config->DataIDMode) &&
            (0x0FFFU < Config->DataID))
   {
      RetVal = E_NOT_OK;
   }
   else if ((E2E_PRN_DATAID_NIBBLE != Config->DataIDMode) &&
       (Config->DataIDNibbleOffset  != 0U))
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtectStateParamChecks
(
   P2CONST(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State
)
{
   Std_ReturnType RetVal;

   /* sanity and input checks: E2E_PRNProtectStateType */
   if (E2EPRN_COUNTER_MAX < State->Counter)
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckNullPtrChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   Std_ReturnType RetVal;

   /* null pointer checks */
   if (NULL_PTR == Config)
   {
      RetVal = E_NOT_OK;
   }
   else if (NULL_PTR == State)
   {
      RetVal = E_NOT_OK;
   }
   else if (NULL_PTR == Data)
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckConfigParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
)
{
   Std_ReturnType RetVal;

   /* sanity and input checks: E2E_PRNConfigType */
   if (E2EPRN_COUNTER_MAX < Config->MaxDeltaCounterInit)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_DATALENGTH_MAX < Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_DATALENGTH_MIN > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (0U != (Config->DataLength % 8U))
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_COUNTER_MAX < Config->SyncCounterInit)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_COUNTER_MAX < Config->MaxNoNewOrRepeatedData)
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->CounterOffset + 4U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->DataIDNibbleOffset + 4U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if ((Config->CRCOffset + 8U) > Config->DataLength)
   {
      RetVal = E_NOT_OK;
   }
   else if (((E2E_PRN_DATAID_BOTH != Config->DataIDMode) &&
             (E2E_PRN_DATAID_LOW != Config->DataIDMode)) &&
             (E2E_PRN_DATAID_NIBBLE != Config->DataIDMode))
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckASRParamChecks
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config
)
{
   Std_ReturnType RetVal;

   if ((E2E_PRN_DATAID_LOW == Config->DataIDMode) &&
            (0x00FFU < Config->DataID))
   {
      RetVal = E_NOT_OK;
   }
   else if ((E2E_PRN_DATAID_NIBBLE == Config->DataIDMode) &&
            (0x0FFFU < Config->DataID))
   {
      RetVal = E_NOT_OK;
   }
   else if ((E2E_PRN_DATAID_NIBBLE != Config->DataIDMode) &&
       (Config->DataIDNibbleOffset  != 0U))
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheckStateParamChecks
(
   P2CONST(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State
)
{
   Std_ReturnType RetVal;

   /* sanity and input checks: E2E_PRNCheckStateType */
   /* State->Status requires no check since it is not read (only written) */
   if (E2EPRN_COUNTER_MAX < State->LastValidCounter)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_COUNTER_MAX < State->MaxDeltaCounter)
   {
      RetVal = E_NOT_OK;
   }
   else if ((TRUE != State->WaitForFirstData) && (FALSE != State->WaitForFirstData))
   {
      RetVal = E_NOT_OK;
   }
   else if ((TRUE != State->NewDataAvailable) && (FALSE != State->NewDataAvailable))
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_COUNTER_MAX < State->SyncCounter)
   {
      RetVal = E_NOT_OK;
   }
   else if (E2EPRN_COUNTER_MAX < State->NoNewOrRepeatedDataCounter)
   {
      RetVal = E_NOT_OK;
   }
   else
   {
      RetVal = E_OK;
   }

   return RetVal;
}

static FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data,
   uint8 CrcByteOffset
)
{
   const uint16 ArrayLength = (uint16)((Config->DataLength / 8U) & 0xFFFFU);
   /* ASCE2E-555:
    * The byte order of the Data ID should be treated as a big endian for CRC Calculation
    * so byte order of the Data Id is reversed */
   const uint8 DataIdHigh  = (uint8)(Config->DataID & 0xFFU);
   const uint8 DataIdLow = (E2E_PRN_DATAID_NIBBLE == Config->DataIDMode) ?
       (uint8)0U : (uint8)(Config->DataID >> 8);
   uint8 Crc = (uint8)0U;

   /* CRC over DataId (low byte)
    * This step is equal for modes: E2E_PRN_DATAID_BOTH,
    * E2E_PRN_DATAID_LOW, and E2E_PRN_DATAID_NIBBLE
    * (mode E2E_PRN_DATAID_ALT is not provided) */
   if (E2E_PRN_DATAID_LOW != Config->DataIDMode)
   {
      Crc = SCrc_CalculateCRC8(&DataIdLow, 1U, E2EPRN_CRC_STARTVALUE, FALSE);
   }

   /* CRC over DataId (high byte)
    * This step only applies for mode: E2E_PRN_DATAID_BOTH
    * (mode E2E_PRN_DATAID_ALT is not supported) */
   if(E2E_PRN_DATAID_LOW == Config->DataIDMode)
   {
      Crc = SCrc_CalculateCRC8(&DataIdHigh, 1U, E2EPRN_CRC_STARTVALUE, FALSE);
   }
   else
   {
      Crc = SCrc_CalculateCRC8(&DataIdHigh, 1U, Crc, FALSE);
   }
   /* CRC over Data. */
   if (0U < CrcByteOffset)
   {
      Crc = SCrc_CalculateCRC8(&Data[0], CrcByteOffset, Crc, FALSE);
   }
   if (((uint16)CrcByteOffset + 1U) < (uint16)ArrayLength)
   {
      Crc = SCrc_CalculateCRC8(&Data[CrcByteOffset + 1U],
                  ((uint32)ArrayLength - ((uint32)CrcByteOffset + 1U)), Crc, FALSE);
   }

   /* Since an XOR-value of 0 is required but the Crc routine uses 0xFFU,
    * the final result must be xored with 0xFFU.
    */
   Crc = Crc ^ E2EPRN_CRC_FINALXORVALUE;

   return Crc;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNProtect
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNProtectStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   const uint8 CrcByteOffset = (uint8)(Config->CRCOffset / 8U);
   const uint8 CntByteOffset = (uint8)(Config->CounterOffset / 8U);
   const uint8 CntBitOffset = (uint8)(Config->CounterOffset % 8U);
   /* Counter goes to low resp. high nibble. */
   const uint8 CntShift = (0U == CntBitOffset) ? 0U : 4U;

   /* Write sequence counter to specified bit offset. */
   /* Bit offset of Counter in MSB order. The offset shall be a multiplicity of 8. */

   Data[CntByteOffset] &= (uint8)(0xF0U >> CntShift);
   Data[CntByteOffset] |= (uint8)((((uint8)(State->Counter) & 0x0FU)) << CntShift);

   if (E2E_PRN_DATAID_NIBBLE == Config->DataIDMode)
   {
      const uint8 DataIDNibbleByteOffset = (uint8)(Config->DataIDNibbleOffset / 8U);
      const uint8 DataIDNibbleBitOffset = (uint8)(Config->DataIDNibbleOffset % 8U);
      /* Data ID Nibble is stored to low resp. high nibble. */
      const uint8 DataIDNibbleShift = (0U == DataIDNibbleBitOffset) ? 0U : 4U;
      const uint8 ConfiguredDataIDNibble =
          (uint8)(((uint16)(Config->DataID >> 8U)) & 0x0FU);

      Data[DataIDNibbleByteOffset] &= (uint8)(0xF0U >> DataIDNibbleShift);
      Data[DataIDNibbleByteOffset] |= (uint8)(ConfiguredDataIDNibble << DataIDNibbleShift);
   }

   /* calculate CRC and write to specified byte offset. */
   /* For E2E_PRN_DATAID_BOTH and E2E_PRN_DATAID_ALT, byte offset is 0. */
   Data[CrcByteOffset] = E2E_EB_CalculateCrc(Config, Data, CrcByteOffset);

   /* update sequence counter modulo 15 */
   if (E2EPRN_COUNTER_MAX > State->Counter)
   {
      State->Counter++;
   }
   else
   {
      State->Counter = 0U;
   }

   return E2E_E_OK;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_PRNCheck
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   Std_ReturnType RetVal;

   if (E2EPRN_COUNTER_MAX > State->MaxDeltaCounter)
   {
      /* MaxDeltaCounter is incremented effectively only
       * if no valid message (CRC or counter not OK) has been
       * received (as it will be reset to the initial value if the
       * received message is valid) */
      State->MaxDeltaCounter++;
   }

   if (TRUE == State->NewDataAvailable)
   {
      const uint8 CrcByteOffset = (uint8)(Config->CRCOffset / 8U);
      const uint8 CntByteOffset = (uint8)(Config->CounterOffset / 8U);
      const uint8 CntBitOffset = (uint8)(Config->CounterOffset % 8U);
      /* Counter is stored to low resp. high nibble. */
      const uint8 CntShift = (0U == CntBitOffset) ? 0U : 4U;
      /* Read sequence counter from specified bit offset. Bit offset of Counter in MSB order. */
      const uint8 ReceivedCounter = (uint8)(((uint8)(Data[CntByteOffset] >> CntShift)) & 0x0FU);
         /* check CRC */
         if (E2E_EB_CalculateCrc(Config, Data, CrcByteOffset) == Data[CrcByteOffset])
         {
            /* Usage of DataID mode Nibble requires additional checks */
            if (E2E_PRN_DATAID_NIBBLE == Config->DataIDMode)
            {
              const uint8 DataIDNibbleByteOffset = (uint8)(Config->DataIDNibbleOffset / 8U);
              const uint8 DataIDNibbleBitOffset = (uint8)(Config->DataIDNibbleOffset % 8U);
              /* Data ID Nibble is stored to low resp. high nibble. */
              const uint8 DataIDNibbleShift = (0U == DataIDNibbleBitOffset) ? 0U : 4U;
              /* Read low nibble of high byte of Data ID from received data via specified bit offset. */
              const uint8 ReceivedDataIDNibble =
                  (uint8)(((uint8)(Data[DataIDNibbleByteOffset] >> DataIDNibbleShift)) & 0x0FU);
              const uint8 ConfiguredDataIDNibble =
                  (uint8)(((uint16)(Config->DataID >> 8U)) & 0x0FU);

              /* check correct DataID Nibble value (if used) */
              if (ReceivedDataIDNibble == ConfiguredDataIDNibble)
              {
                 E2E_EB_PRNCheckCounter(Config, State, ReceivedCounter);
              }
              else
              {
                /* wrong CRC */
                State->Status = E2E_PRNSTATUS_WRONGCRC;
              }
            }
            else
            {
               E2E_EB_PRNCheckCounter(Config, State, ReceivedCounter);
            }
         }
         else
         {
            /* wrong CRC */
            State->Status = E2E_PRNSTATUS_WRONGCRC;
         }
         RetVal = E2E_E_OK;
   }
   else
   {
      /* Increment counter for consecutively missing or repeated Data */
      if (E2EPRN_COUNTER_MAX > State->NoNewOrRepeatedDataCounter)
      {
         State->NoNewOrRepeatedDataCounter++;
      }
      /* no new data available since the last call */
      State->Status = E2E_PRNSTATUS_NONEWDATA;
      RetVal = E2E_E_OK;
   }

   return RetVal;
}

static FUNC(void, E2E_CODE) E2E_EB_PRNCheckCounter
(
   P2CONST(E2E_PRNConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_PRNCheckStateType, AUTOMATIC, E2E_APPL_DATA) State,
   uint8 ReceivedCounter
)
{
   if (TRUE == State->WaitForFirstData)
   {
      /* first data received since init or reinit
       * (sequence counter cannot be checked) */
      State->WaitForFirstData = FALSE;
      State->MaxDeltaCounter = Config->MaxDeltaCounterInit;
      State->LastValidCounter = ReceivedCounter;
      State->Status = E2E_PRNSTATUS_INITIAL;
   }
   else
   {
      /* check sequence counter */
      const uint8 DeltaCounter = (ReceivedCounter >= State->LastValidCounter) ?
         (uint8)(ReceivedCounter - State->LastValidCounter) :
         (uint8)((ReceivedCounter + E2EPRN_COUNTER_MAX + 1U)
             - State->LastValidCounter);

      /* The previous and the current data have correct CRC, verify the counter. */
      if (DeltaCounter > State->MaxDeltaCounter)
      {
         /* An unexpected behavior of the counter was detected,
          * so a statement about missing or repeated Data makes no sense anymore */
         State->NoNewOrRepeatedDataCounter = 0U;
         /* Ensure a deterministic behavior of
          * the detected communication errors. */
         State->SyncCounter = Config->SyncCounterInit;
         /* A configured SyncCounterInit value of 0 ensures
          * backward compatibility to ASR 4.0 Rev 3 */
         if (0U < Config->SyncCounterInit)
         {
            State->MaxDeltaCounter = Config->MaxDeltaCounterInit;
            State->LastValidCounter = ReceivedCounter;
         }
         /* counter too big, i.e. too many data in the sequence
          * have been probably lost since the last correct/initial
          * reception. */
         State->Status = E2E_PRNSTATUS_WRONGSEQUENCE;
      }
      else if (1U < DeltaCounter)
      {
         /* the Counter is incremented by DeltaCounter
          * (1 <= DeltaCounter <= MaxDeltaCounter), i.e.
          * some Data in the sequence have been probably lost since
          * the last correct/initial reception, but this is within
          * the configured tolerance range. */
         State->MaxDeltaCounter = Config->MaxDeltaCounterInit;
         State->LastValidCounter = ReceivedCounter;
         State->LostData = DeltaCounter - 1U;
         if (Config->MaxNoNewOrRepeatedData >= State->NoNewOrRepeatedDataCounter)
         {
            /* Number of missing or repeated Data is tolerated,
             * so return status OKSOMELOST or SYNC if a re-synchronization
             * process is ongoing. */
            if (0U < State->SyncCounter)
            {
               State->SyncCounter--;
               State->Status = E2E_PRNSTATUS_SYNC;
            }
            else
            {
               State->Status = E2E_PRNSTATUS_OKSOMELOST;
            }
         }
         else
         {
            /* The limit of missing or repeated Data exceeded
             * => start re-synchronization handling for a deterministic behavior */
            State->SyncCounter = Config->SyncCounterInit;
            State->Status = E2E_PRNSTATUS_SYNC;
         }
         State->NoNewOrRepeatedDataCounter = 0U;
      }
      else if (1U == DeltaCounter)
      {
         /* the Counter is incremented by 1, i.e. no Data has
          * been lost since the last correct data reception. */
         State->MaxDeltaCounter = Config->MaxDeltaCounterInit;
         State->LastValidCounter = ReceivedCounter;
         State->LostData = 0U;
         if (Config->MaxNoNewOrRepeatedData >= State->NoNewOrRepeatedDataCounter)
         {
            /* Number of missing or repeated Data is tolerated,
             * so return status OK or SYNC if a re-synchronization
             * process is ongoing. */
            if (0U < State->SyncCounter)
            {
               State->SyncCounter--;
               State->Status = E2E_PRNSTATUS_SYNC;
            }
            else
            {
               State->Status = E2E_PRNSTATUS_OK;
            }
         }
         else
         {
            /* The limit of missing or repeated Data exceeded
             * => start re-synchronization handling for a deterministic behavior */
            State->SyncCounter = Config->SyncCounterInit;
            State->Status = E2E_PRNSTATUS_SYNC;
         }
         State->NoNewOrRepeatedDataCounter = 0U;
      }
      else /* DeltaCounter == 0U */
      {
         /* the counter is identical, i.e. data has already been
          * received (repeated message) */
         /* Increment counter for consecutively missing or repeated Data */
         if (E2EPRN_COUNTER_MAX > State->NoNewOrRepeatedDataCounter)
         {
            State->NoNewOrRepeatedDataCounter++;
         }
         State->Status = E2E_PRNSTATUS_REPEATED;
      }
   }
}
#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
