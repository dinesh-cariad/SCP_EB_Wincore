/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COM_CALLOUT_H_
#define COM_CALLOUT_H_
/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard type */
#include <Com_Cfg.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
/** \brief Define Com_TxCalloutType */
typedef FUNC(boolean, COM_APPL_CODE) (Com_TxCalloutType)
(
   PduIdType ID,
   P2VAR(PduInfoType, AUTOMATIC, COM_VAR_CLEARED) PduInfoPtr
);

/** \brief Define Com_RxCalloutType */
typedef FUNC(boolean, COM_APPL_CODE) (Com_RxCalloutType)
(
   PduIdType ID,
   P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
);
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /*COM_CALLOUT_H_ */
/*==================[end of file]===========================================*/

