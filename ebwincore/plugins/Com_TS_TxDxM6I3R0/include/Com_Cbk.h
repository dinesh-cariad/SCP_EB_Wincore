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
#ifndef COM_CBK_H
#define COM_CBK_H
/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */

#include <Com_Cbk_Static.h>     /* Include static part */
#include <Com_Version.h>        /* Module version declarations */

#include <Com_Local_Cfg.h>      /* local configuration
                                   (switch for SWS 2.1 or SWS 3.0) */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/** \brief Define Com_CbkTxAck_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkTxAck_Type)
(
   void
);

/** \brief Define Com_CbkTxErr_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkTxErr_Type)
(
   void
);

/** \brief Define Com_CbkTxTOut_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkTxTOut_Type)
(
   void
);

/** \brief Define Com_CbkRxAck_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkRxAck_Type)
(
   void
);

/** \brief Define Com_CbkRxTOut_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkRxTOut_Type)
(
   void
);

/** \brief Define Com_CbkInv_Type */
typedef FUNC(void, COM_APPL_CODE) (Com_CbkInv_Type)
(
   void
);

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COM_CBK_H */
/*==================[end of file]===========================================*/
