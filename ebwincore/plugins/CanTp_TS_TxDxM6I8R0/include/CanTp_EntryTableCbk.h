/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_ENTRYTABLECBK_H
#define CANTP_ENTRYTABLECBK_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* typedefs for AUTOSAR com stack */
#include <CanTp_Cfg.h>              /* CanTp configuration */

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_OFF)
#include <CanTp_EntryTable.h>       /* CanTp entry jumptable */
#endif

/*==================[macros]=================================================*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_OFF)

#if (defined CanTp_RxIndication) /* to prevent double declaration */
#error CanTp_RxIndication is already defined
#endif /* if (defined CanTp_RxIndication) */

/** \brief Wrapper macro for callback function CanTp_RxIndication() */
#define CanTp_RxIndication    (CanTp_EntryTablePtr->RxIndication)

#if (defined CanTp_TxConfirmation) /* to prevent double declaration */
#error CanTp_TxConfirmation is already defined
#endif /* if (defined CanTp_TxConfirmation) */

/** \brief Wrapper macro for callback function CanTp_TxConfirmation() */
#define CanTp_TxConfirmation  (CanTp_EntryTablePtr->TxConfirmation)

#endif /*if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON) */
/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_ENTRYTABLECBK_H */
/*==================[end of file]============================================*/
