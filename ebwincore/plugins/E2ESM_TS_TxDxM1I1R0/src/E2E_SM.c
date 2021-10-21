/**
 * \file
 *
 * \brief AUTOSAR E2ESM
 *
 * This file contains the implementation of the AUTOSAR
 * module E2ESM.
 *
 * \version 1.1.10
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <E2E_SM.h>       /* E2E State Machine */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

#define E2E_START_SEC_CODE
#include <E2E_MemMap.h>

/**
 * \brief Perform null pointer checks of the input parameters of the Check function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr  Pointer to port/data communication state.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckNullChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Perform input parameter checks of the ConfigPtr parameter of the Check function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 *
 * \return Result of input parameter check
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckConfigParamChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
);

/**
 * \brief Perform input parameter checks of the StatePtr parameter of the Check function
 *
 * \param[in] ConfigPtr Pointer to static configuration.
 * \param[in] StatePtr Pointer to port/data communication state.
 *
 * \return \a StatePtr has a valid State Machine status
 * \retval E_OK State Machine status is valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckStateParamChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Perform input parameter checks of the ProfileStatus parameter of the Check function
 *
 * \param[in] ProfileStatus  Profile Status
 *
 * \return \a ProfileStatus has a valid state
 * \retval E_OK all input parameter are valid
 * \retval E_NOT_OK At least on input parameter is invalid
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckProfileStatusChecks
(
    E2E_PCheckStatusType ProfileStatus
);

/**
 * \brief Check the received Data using the E2E Profile SM.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in]     ConfigPtr     Pointer to static configuration.
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_WRONGSTATE Function executed in wrong state.
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheck
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Handle E2E State Machine transitions from E2E_SM_NODATA state.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMNoData
(
    E2E_PCheckStatusType ProfileStatus,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Handle E2E State Machine transitions from E2E_SM_INIT state.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in]     ConfigPtr     Pointer to static configuration.
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMInit
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Handle E2E State Machine transitions from E2E_SM_VALID state.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in]     ConfigPtr     Pointer to static configuration.
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMValid
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Handle E2E State Machine transitions from E2E_SM_INVALID state.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in]     ConfigPtr     Pointer to static configuration.
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return Function execution success status
 * \retval E2E_E_OK Function completed successfully.
 */
static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMInvalid
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/**
 * \brief Handle E2E State Machine transitions from E2E_SM_NODATA state.
 *
 * \param[in]     ProfileStatus Profile-independent status of the reception on one single
 *                              Data in one cycle
 * \param[in]     ConfigPtr     Pointer to static configuration.
 * \param[in,out] StatePtr      Pointer to port/data communication state.
 *
 * \pre All input parameters are valid
 *
 * \return None
 */
static FUNC(void, E2E_CODE) E2E_EB_SMAddStatus
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
);

/*==================[internal constants]=====================================*/

/*==================[internal data]==========================================*/

/*==================[external function definition]=========================*/

FUNC(Std_ReturnType, E2E_CODE) E2E_SMCheck
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal = E2E_E_INTERR;

    if (E2E_EB_SMCheckNullChecks(ConfigPtr, StatePtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_SMCheckConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else if (E2E_EB_SMCheckStateParamChecks(ConfigPtr, StatePtr) != E_OK)
    {
        RetVal = E2E_E_WRONGSTATE;
    }
    else if (E2E_EB_SMCheckProfileStatusChecks(ProfileStatus) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        RetVal = E2E_EB_SMCheck(ProfileStatus, ConfigPtr, StatePtr);
    }

    return RetVal;
}

FUNC(Std_ReturnType, E2E_CODE) E2E_SMCheckInit
(
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal = E2E_E_INTERR;

    if (E2E_EB_SMCheckNullChecks(ConfigPtr, StatePtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_NULL;
    }
    else if (E2E_EB_SMCheckConfigParamChecks(ConfigPtr) != E_OK)
    {
        RetVal = E2E_E_INPUTERR_WRONG;
    }
    else
    {
        uint8 Idx;

        for(Idx = 0U; Idx < ConfigPtr->WindowSize; Idx++)
        {
            StatePtr->ProfileStatusWindowArray[Idx] = E2E_P_NOTAVAILABLE;
        }
        StatePtr->WindowTopIndex = 0U;
        StatePtr->OkCount = 0U;
        StatePtr->ErrorCount = 0U;
        StatePtr->SMState = E2E_SM_NODATA;

        RetVal = E2E_E_OK;
    }

    return RetVal;
}

/*==================[internal function definition]===========================*/

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckNullChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal = E_NOT_OK;

    /* null pointer checks */
    if (NULL_PTR == ConfigPtr)
    {
        RetVal = E_NOT_OK;
    }
    else if (NULL_PTR == StatePtr)
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckConfigParamChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr
)
{
    Std_ReturnType RetVal = E_NOT_OK;
    /* sanity and input checks of configuration data: E2E_SMConfigType */
    if ((ConfigPtr->WindowSize == 0U) || (ConfigPtr->WindowSize > E2ESM_MAX_WINDOW_SIZE))
    {
        RetVal = E_NOT_OK;
    }
    else if (
              ConfigPtr->WindowSize <
              (ConfigPtr->MinOkStateValid + ConfigPtr->MaxErrorStateValid)
            )
    {
        RetVal = E_NOT_OK;
    }
    else if (
              ConfigPtr->WindowSize <
              (ConfigPtr->MinOkStateInit + ConfigPtr->MaxErrorStateInit)
            )
    {
        RetVal = E_NOT_OK;
    }
    else if (
              ConfigPtr->WindowSize <
              (ConfigPtr->MinOkStateInvalid + ConfigPtr->MaxErrorStateInvalid)
            )
    {
        RetVal = E_NOT_OK;
    }
    else if (
              (ConfigPtr->MaxErrorStateInvalid > ConfigPtr->MaxErrorStateInit) ||
              (ConfigPtr->MaxErrorStateInit > ConfigPtr->MaxErrorStateValid)
            )
    {
        RetVal = E_NOT_OK;
    }
    else if (
              (ConfigPtr->MinOkStateValid == 0U) ||
              (
                (ConfigPtr->MinOkStateValid > ConfigPtr->MinOkStateInit) ||
                (ConfigPtr->MinOkStateInit > ConfigPtr->MinOkStateInvalid)
              )
            )
    {
        RetVal = E_NOT_OK;
    }
    else
    {
        RetVal = E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckStateParamChecks
(
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2CONST(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal = E_NOT_OK;

    /* sanity and input checks of stated data: E2E_SMCheckStateType
     * ProfileStatus is read and therefore checked here
     * SMState : Check that value is in range */
    switch (StatePtr->SMState)
    {
    case E2E_SM_NODATA: /* fall through */
    case E2E_SM_INIT: /* fall through */
    case E2E_SM_VALID: /* fall through */
    case E2E_SM_INVALID: /* fall through */
        RetVal = E_OK;
        break;
    default: /* E2E_SM_DEINIT shall result into E2E_E_WRONGSTATE */
        RetVal = E_NOT_OK;
        break;
    }

    /* Check if WindowTopIndex is greater or equal to WindowSize */
    if (StatePtr->WindowTopIndex >= ConfigPtr->WindowSize)
    {
        RetVal = E_NOT_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheckProfileStatusChecks
(
    E2E_PCheckStatusType ProfileStatus
)
{
    Std_ReturnType RetVal = E_NOT_OK;

    /* sanity and input checks of stated data: E2E_SMCheckStateType
     * ProfileStatusWindowArray, WindowTopIndex, OkCount, ErrorCount : All values are valid
     * SMState : Check that value is in range */
    switch (ProfileStatus)
    {
    case E2E_P_OK: /* fall through */
    case E2E_P_ERROR: /* fall through */
    case E2E_P_WRONGSEQUENCE: /* fall through */
    case E2E_P_REPEATED: /* fall through */
    case E2E_P_NONEWDATA: /* fall through */
        RetVal = E_OK;
        break;
    default: /* E2E_P_NOTAVAILABLE shall result into E2E_E_INPUTERR_WRONG */
        RetVal = E_NOT_OK;
        break;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_SMCheck
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal = E2E_E_INTERR;

    /* calculate Next State value */
    switch (StatePtr->SMState)
    {
    case E2E_SM_NODATA:
    {
        RetVal = E2E_EB_CheckHandleSMNoData(ProfileStatus, StatePtr);
        break;
    }
    case E2E_SM_INIT:
        {
            RetVal = E2E_EB_CheckHandleSMInit(ProfileStatus, ConfigPtr, StatePtr);
            break;
        }
    case E2E_SM_VALID:
    {
        RetVal = E2E_EB_CheckHandleSMValid(ProfileStatus, ConfigPtr, StatePtr);
        break;
    }
    case E2E_SM_INVALID:
    {
        RetVal = E2E_EB_CheckHandleSMInvalid(ProfileStatus, ConfigPtr, StatePtr);
        break;
    }
    /* CHECK: NOPARSE */
    /* Rational to not use a MOD_UNREACHABLE_CODE_ASSERT statement:
       No Det is available for safety reasons and, thus, also not for the defensive programming
       techniques applied at EB (e.g. unreachable code macros) */
    default:
        /* shall never be reached, defensive programming */
        RetVal = E2E_E_INTERR;
        break;
    /* CHECK: PARSE */
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMNoData
(
    E2E_PCheckStatusType ProfileStatus,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    if ((ProfileStatus != E2E_P_ERROR) && (ProfileStatus != E2E_P_NONEWDATA))
    {
        StatePtr->SMState = E2E_SM_INIT;
        RetVal = E2E_E_OK;
    }
    else
    {
        StatePtr->SMState = E2E_SM_NODATA;
        RetVal = E2E_E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMInit
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    E2E_EB_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);
    if (StatePtr->ErrorCount > ConfigPtr->MaxErrorStateInit)
    {
        StatePtr->SMState = E2E_SM_INVALID;
        RetVal = E2E_E_OK;
    }
    else if (StatePtr->OkCount >= ConfigPtr->MinOkStateInit)
    {
        StatePtr->SMState = E2E_SM_VALID;
        RetVal = E2E_E_OK;
    }
    else
    {
        StatePtr->SMState = E2E_SM_INIT;
        RetVal = E2E_E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMValid
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    E2E_EB_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);

    if (
         (StatePtr->ErrorCount > ConfigPtr->MaxErrorStateValid) ||
         (StatePtr->OkCount < ConfigPtr->MinOkStateValid)
       )
    {
        StatePtr->SMState = E2E_SM_INVALID;
        RetVal = E2E_E_OK;
    }
    else
    {
        StatePtr->SMState = E2E_SM_VALID;
        RetVal = E2E_E_OK;
    }

    return RetVal;
}

static FUNC(Std_ReturnType, E2E_CODE) E2E_EB_CheckHandleSMInvalid
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    Std_ReturnType RetVal;

    E2E_EB_SMAddStatus(ProfileStatus, ConfigPtr, StatePtr);

    if (
         (StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateInvalid) &&
         (StatePtr->OkCount >= ConfigPtr->MinOkStateInvalid)
       )
    {
        StatePtr->SMState = E2E_SM_VALID;
        RetVal = E2E_E_OK;
    }
    else
    {
        StatePtr->SMState = E2E_SM_INVALID;
        RetVal = E2E_E_OK;
    }

    return RetVal;
}

static FUNC(void, E2E_CODE) E2E_EB_SMAddStatus
(
    E2E_PCheckStatusType ProfileStatus,
    P2CONST(E2E_SMConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
    P2VAR(E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr
)
{
    uint8 Idx;
    uint8 okCountTemp = 0U;
    uint8 errorCountTemp = 0U;

    StatePtr->ProfileStatusWindowArray[StatePtr->WindowTopIndex] = ProfileStatus;

    for(Idx = 0U; Idx < ConfigPtr->WindowSize; Idx++)
    {
        switch (StatePtr->ProfileStatusWindowArray[Idx])
        {
            case E2E_P_OK:
                okCountTemp++;
                break;
            case E2E_P_REPEATED: /* fall through */
            case E2E_P_WRONGSEQUENCE: /* fall through */
            case E2E_P_NOTAVAILABLE: /* fall through */
            case E2E_P_NONEWDATA: /* do not count */
                break;
            default: /* E2E_P_ERROR or undefined state */
                errorCountTemp++;
                break;
        }
    }

    StatePtr->OkCount = okCountTemp;
    StatePtr->ErrorCount = errorCountTemp;

    if (StatePtr->WindowTopIndex == (ConfigPtr->WindowSize - 1U))
    {
        StatePtr->WindowTopIndex = 0U;
    }
    else
    {
        StatePtr->WindowTopIndex++;
    }
}

#define E2E_STOP_SEC_CODE
#include <E2E_MemMap.h>

/*==================[end of file]============================================*/
