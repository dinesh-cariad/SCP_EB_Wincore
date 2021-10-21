#ifndef STBM_ETHTSYN_H
#define STBM_ETHTSYN_H

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


#if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)

#if (defined STBM_API_ETHSETMASTERTIMINGDATA)
#error STBM_API_ETHSETMASTERTIMINGDATA is already defined.
#endif
/** \brief Service ID for StbM_EthSetMasterTimingData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00492,1 */
#define STBM_API_ETHSETMASTERTIMINGDATA                  0x22U


#if (defined STBM_API_ETHSETSLAVETIMINGDATA)
#error STBM_API_ETHSETSLAVETIMINGDATA is already defined.
#endif
/** \brief Service ID for StbM_EthSetSlaveTimingData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00486,1 */
#define STBM_API_ETHSETSLAVETIMINGDATA                   0x28U


#if (defined STBM_API_ETHSETPDELAYINITIATORDATA)
#error STBM_API_ETHSETPDELAYINITIATORDATA is already defined.
#endif
/** \brief Service ID for StbM_EthSetPdelayInitiatorData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00496,1 */
#define STBM_API_ETHSETPDELAYINITIATORDATA                  0x23U


#if (defined STBM_API_ETHSETPDELAYRESPONDERDATA)
#error STBM_API_ETHSETPDELAYRESPONDERDATA is already defined.
#endif
/** \brief Service ID for StbM_EthSetPdelayResponderData() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00500,1 */
#define STBM_API_ETHSETPDELAYRESPONDERDATA                   0x24U

#endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */


/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>


#if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
/** \brief Service to set ETH Master Timing Data
 **
 ** Allows the ETHTSYN module to provide specific data for a Time Master to the StbM.
 ** This function will be used if a Time Master is present in this ECU and if it is linked to a ETHTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - Measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x22}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00492,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_EthSetMasterTimingData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_EthTimeMasterMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);


/** \brief Service to set ETH Slave Timing Data
 **
 ** Allows the ETHTSYN module to forward ETH specific details for a Time Slave to the StbM.
 ** This function will be used if a Time Slave is present in this ECU and if it is linked to a ETHTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - New measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x28}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00486,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_EthSetSlaveTimingData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_EthTimeSlaveMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);



/** \brief Service to set ETH Slave Pdelay Timing Data
 **
 ** Allows the ETHTSYN module to forward ETH specific details for a Time Slave to the StbM.
 ** This function will be used if a Time Slave is present in this ECU and if it is linked to a ETHTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - Measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x23}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00496,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_EthSetPdelayInitiatorData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_PdelayInitiatorMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);


/** \brief Service to set ETH Master Pdelay Timing Data
 **
 ** Allows the ETHTSYN module to forward ETH specific details for a Time Master to the StbM.
 ** This function will be used if a Time Master is present in this ECU and if it is linked to a ETHTSYN Time Domain.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] measureDataPtr - Measurement data
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x24}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00500,1 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_EthSetPdelayResponderData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_PdelayResponderMeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr
);
#endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>


#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_ETHTSYN_H */
