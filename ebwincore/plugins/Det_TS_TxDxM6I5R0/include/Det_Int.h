/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DET_INT_H
#define DET_INT_H

/*==================[inclusions]============================================*/

#include <Det_Cfg.h>

/*==================[macros]================================================*/

/** \brief The number of default error types */
#define DET_NR_OF_ERROR_TYPES 3U

/** \brief Index for writing development errors into the buffer*/
#define DET_DEVELOPMENT_ERROR_TYPE 0U

/** \brief Index for writing runtime errors into the buffer*/
#define DET_RUNTIME_ERROR_TYPE 1U

/** \brief Index for writing transient faults into the buffer*/
#define DET_TRANSIENT_FAULT_TYPE 2U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* An exclusive area is only needed for internal logging within
 * the single-core implementation of the Det module */
#if (DET_MULTICORE_ENABLED == FALSE)
#if (DET_LOGMODE == DET_LOGMODE_INTERNAL)
#define DET_START_SEC_CODE
#include <Det_MemMap.h>

/** \brief Proxy function for SchM_Enter_Det_SCHM_DET_EXCLUSIVE_AREA_0
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation unit Det.c
 **
 ** \return No return value. */
extern FUNC(void, DET_CODE) Det_Enter_SCHM_DET_EXCLUSIVE_AREA_0 (void);

/** \brief Proxy function for SchM_Exit_Det_SCHM_DET_EXCLUSIVE_AREA_0
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation unit Det.c
 **
 ** \return No return value. */
extern FUNC(void, DET_CODE) Det_Exit_SCHM_DET_EXCLUSIVE_AREA_0 (void);


/** \brief Stop execution function.
 **
 ** This function stop execution after a development error is reported.
 **
 ** \return No return value. */
extern FUNC(void, DET_CODE) Det_StopExecution(uint8 ErrorType);

#define DET_STOP_SEC_CODE
#include <Det_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* DET_LOGMODE == DET_LOGMODE_INTERNAL */
#endif /* DET_MULTICORE_ENABLED == FALSE */

/*==================[end of file]===========================================*/
#endif /* ifndef DET_INT_H */
