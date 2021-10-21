/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_TYPES_H
#define DOIP_TYPES_H
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00158, 1 */

/*==================[includes]==============================================*/

#include <ComStack_Types.h>
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00157, 1 */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Authentication / confirmation callback function return code **/
#if (defined DOIP_E_PENDING)
#error DOIP_E_PENDING is already defined
#endif
#define DOIP_E_PENDING 0x10U
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00272, 1 */
/* !LINKSTO DoIP.ASR41.SWS_DoIP_00273, 1 */

/* === DoIP Interface Types for MeasurementIdx === */
/** \brief Constant for the value of Measurement index for dropped messages caused by invalid
 *         generic header */
#if (defined DOIP_MEAS_DROP_GENHDR)
#error DOIP_MEAS_DROP_GENHDR is already defined
#endif
#define DOIP_MEAS_DROP_GENHDR 1U

/** \brief Constant for the value of Measurement index for dropped messages caused by invalid
 *         diagnostic header */
#if (defined DOIP_MEAS_DROP_DIAGMSG)
#error DOIP_MEAS_DROP_DIAGMSG is already defined
#endif
#define DOIP_MEAS_DROP_DIAGMSG 2U

/** \brief Constant for the value of Measurement index that represents all measurement indexes */
#if (defined DOIP_MEAS_ALL)
#error DOIP_MEAS_ALL is already defined
#endif
#define DOIP_MEAS_ALL 0xFFU

/** \brief Definition of DoIP_MeasurementIdxType - index to select specific measurement data */
typedef uint8 DoIP_MeasurementIdxType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* DOIP_TYPES_H */
/*==================[end of file]===========================================*/

