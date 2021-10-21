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
#ifndef COM_FILTER_H
#define COM_FILTER_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Rules.h>          /* Generated configuration + derived rules */

#if (COM_FILTER_AVAILABLE == STD_ON)
#include <Com_Api.h>            /* Module public API */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterBoolean
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterUint8
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterSint8
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterUint16
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterSint16
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterUint32
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);


TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterSint32
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterUint64
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_FilterSint64
(
   P2CONST(ComFilterConfType, AUTOMATIC, AUTOMATIC) ComFilterPropPtr,
   P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
);

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#endif /* (COM_FILTER_AVAILABLE == STD_ON) */
#endif /* COM_FILTER_H */
/*==================[end of file]===========================================*/

