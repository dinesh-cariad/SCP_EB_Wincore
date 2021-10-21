/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.MetaData.PrivateApi,1 */
/* This file contains the data types, macro definitions and provides external function declarations used by MetaData software unit. */

#if (!defined DCM_DSL_COMMUNICATIONSERVICES_METADATA_H)
#define DCM_DSL_COMMUNICATIONSERVICES_METADATA_H

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                              /* AUTOSAR standard types */
#include <Dcm_Cfg.h>                                     /* Dcm configuration header file */
#include <Dcm_Dsl_Cfg.h>

#include <Dcm_Dsl_CommunicationServices.h>               /* Dcm Dsl Communication Services header file */
#include <Dcm_Dsl_CommunicationServices_RxConnections.h>
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/
#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.MetaData.RuntimeDataTypes, 1 */
/** \brief This type contains the information for each generic connection of the Dcm module. */
typedef struct
{
  /** \brief Target Address used for generic connection handling.
   */
  uint16 TargetAddress;

  /** \brief Source Address used for generic connection handling.
   */
  uint16 SourceAddress;
}
Dcm_PduInfoType;
/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/


#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

extern VAR(Dcm_PduInfoType, DCM_VAR) Dcm_PduInfo[DCM_NUM_GENERIC_CONNECTIONS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <MemMap.h>

/** \brief             Function used to extract information from EcuC related to MetaData
 **                    Source and Target address
 **
 ** \param[in]         Dcm_MainConnectionIndexType MainConnectionIdx for which information
 **                    needs to be obtained.
 **
 ** \retval E_OK       Operation completed.
 ** \retval E_NOT_OK   Source/Target address could not be obtained.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_GetEcuCMetaDataInfo(
  PduIdType RxPduIdx
);

/** \brief             Function used to provide information to EcuC related to MetaData
 **                    Source and Target address
 **
 ** \param[in]         EcuPduId - the Id which needs to be transmitted to EcuC
 **                    in order to update MetaDataInformation.
 ** \param[in]         GenericConIdx - the index of the generic connection
 **                    for which information needs to be updated.
 ** \param[inout]      PduInfoPtr - Pdu information needed by EcuC in order to update
 **                    MetaDataInformation.
 **
 ** \retval E_OK       Operation completed.
 ** \retval E_NOT_OK   Source/Target address could not be set.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_SetEcuCMetaDataInfo(
  PduIdType EcuPduId,
  Dcm_MainConnectionIndexType GenericConIdx,
  P2VAR(PduInfoType, AUTOMATIC, EBSTUBS_APPL_CONST) PduInfoPtr
);

#define DCM_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function definitions]==============================================*/

#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */
#endif /* #if (!defined DCM_DSL_COMMUNICATIONSERVICES_METADATA_H) */
/*==================[end of file]================================================================*/
