/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRIF_TYPES_H
#define FRIF_TYPES_H

/*======================[Include File Checking Section]=======================*/

/*======================[Include Section]=====================================*/

/*======================[Global Macros]=======================================*/

/*======================[Global Data Types]===================================*/

/**
 * \brief Definition of FrIf state type.
 *
 * This enumerator holds all states the FrIf offers to upper modules.
 */
typedef enum
{
    FRIF_STATE_OFFLINE = 0,
    /**< HALT-State, FlexRay Controller is synchronized, Communication is blocked   */
    FRIF_STATE_ONLINE
    /**< ONLINE-State, FlexRay Controller is synchronized, Communication is enabled   */
}FrIf_StateType;

/**
 * \brief Definition of FrIf state transition type.
 *
 * This enumerator holds all state-transistions the FrIf offers to upper modules.
 */
typedef enum
{
    FRIF_GOTO_ONLINE = 0,
    /**< move to state ONLINE from any other state */
    FRIF_GOTO_OFFLINE
    /**< move to state OFFLINE from any other state */
}FrIf_StateTransitionType;

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

#endif /* FRIF_TYPES_H */
