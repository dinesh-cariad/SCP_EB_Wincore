#ifndef STBM_FRTSYN_H
#define STBM_FRTSYN_H

/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)

/*==[Includes]================================================================*/

#include <StbM_Int_Cfg.h>

/*==[Macros]==================================================================*/

/* API service IDs */


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON)

#if (defined STBM_API_FRSETMASTERTIMINGDATA)
#error STBM_API_FRSETMASTERTIMINGDATA is already defined.
#endif
/** \brief Service ID for StbM_FrSetMasterTimingData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00491,1 */
#define STBM_API_FRSETMASTERTIMINGDATA                  0x21U

#endif /* #if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON) */


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON)

#if (defined STBM_API_FRSETSLAVETIMINGDATA)
#error STBM_API_FRSETSLAVETIMINGDATA is already defined.
#endif
/** \brief Service ID for StbM_FrSetSlaveTimingData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00485,1 */
#define STBM_API_FRSETSLAVETIMINGDATA                   0x27U

#endif /* #if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON) */


/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON)

/** \brief Service to set FR Master Timing Data
 **
 ** Allows the FRTSYN module to provide specific data for a Time Master to the StbM.
 ** This function will be used if a Time Master is present in this ECU and if it is linked to a FRTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - Measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x21}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00491,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_FrSetMasterTimingData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_FrTimeMasterMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);

#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON) */


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON)

/** \brief Service to set FR Slave Timing Data
 **
 ** Allows the FRTSYN module to forward FR specific details for a Time Slave to the StbM.
 ** This function will be used if a Time Slave is present in this ECU and if it is linked to a FRTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - New measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x27}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00485,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_FrSetSlaveTimingData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_FrTimeSlaveMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);

#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON) */


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_FRTSYN_H */
