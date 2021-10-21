/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef NVM_TYPES_H
#define NVM_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>            /* EB specific standard types      */
/* !LINKSTO NVM690,1 */
#include <Std_Types.h>            /* AUTOSAR standard types. NVM550 */
#include <NvM_BSW_Types.h>
[!/*
   If RTE Usage is enabled, Include Rte_NvM_Type.h to get the
   NvM type definitions.
*/!]
[!IF "NvMCommon/NvMRteUsage = 'true'"!][!//
/* !LINKSTO NVM755,1 */
#include <Rte_NvM_Type.h>    /* Rte type definitions. */
[!ENDIF!][!//

/*==================[type definitions]======================================*/

[!IF "(NvMCommon/NvMRteUsage = 'false') or (NvMCommon/NvMServiceAPI/NvMDefaultASRServiceAPI = 'NONE')"!][!//
/** \brief Provides AUTOSAR 4.0 NvM_RequestResultType as default to other
 ** BSW modules */
/* !LINKSTO NVM471,1 */
typedef NvM_ASR40_BlockIdType NvM_BlockIdType;
/** \brief Provides AUTOSAR 4.0 NvM_RequestResultType as default to other
 ** BSW modules */
/* !LINKSTO SWS_NvM_00470,2 */
typedef NvM_ASR40_RequestResultType NvM_RequestResultType;

[!ENDIF!][!//

/**  \brief Defines the NVRAM block initialization call back function.
 **         It is called if data from NVRAM is not readable.
 **/
typedef P2FUNC( Std_ReturnType,NVM_CODE,NvM_InitBlockCallbackType )( void );

/**  \brief Defines the NVRAM block error status callback function.
 **         It is called if a asynchronous NvM API request has finished.
 **/
typedef P2FUNC( Std_ReturnType,NVM_CODE,NvM_SingleBlockCallbackType )
              ( uint8 ServiceId, NvM_RequestResultType JobResult );

/**  \brief Defines the NVRAM multi request callback function.
 **         It is called if the NvM multiblock requests
 **         NvM_WriteAll() or NvM_ReadAll() or NvM_ValidateAll() is finished.
 **/
typedef P2FUNC( void,NVM_CODE,NvM_MultiBlockCallbackType )
              ( uint8 ServiceId, NvM_RequestResultType JobResult );

[!IF "count(NvMBlockDescriptor/*[(NvMBlockUseSyncMechanism = 'true')]) != 0"!][!//
/**  \brief Defines the RAM to NvM copy callback function.
 **         It is called in order to let the application copy data from
 **         RAM block to NvM module's mirror.
 **/

typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_RamToNvCopyCallbackType)
              (P2VAR(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);

/**  \brief Defines the NvM to RAM copy callback function.
 **         It is called in order to let the application copy data from
 **         NvM module's mirror to RAM block.
 **/
typedef P2FUNC(Std_ReturnType, NVM_APPL_CODE, NvM_NvToRamCopyCallbackType)
              (P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvMBuffer);
[!ENDIF!]

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* NVM_TYPES_H */

/*=========================[end of file NvM_Types.h]========================*/
