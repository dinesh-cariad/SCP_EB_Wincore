/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/
#include <TSMem.h>                /* TS_MemCpy(), TS_MemSet() */
#include <EcuC.h>

#if ECUC_META_DATA_HANDLING_ENABLED == STD_ON
#include <ComStack_Helpers.h>          /* Com Stack Helper macros */
#endif /* ECUC_META_DATA_HANDLING_ENABLED == STD_ON */
#if ECUC_DEV_ERROR_DETECT == STD_ON
#include <Det.h>                       /* Det function declaration */

/*==================[macros]=================================================*/

/** \brief AUTOSAR module identification */
#define ECUC_MODULE_ID                                                   10U

/** \brief Instance Id for DET 2.0 */
#define ECUC_INSTANCE_ID                                                  0U

/** \brief Macro for DET calls in ECUC development-error-detection
 *         sections (Det enabled) */
#define ECUC_REPORT_ERROR(ApiId, ErrorId)                      \
  (void) Det_ReportError(ECUC_MODULE_ID, ECUC_INSTANCE_ID, (ApiId), (ErrorId))

#endif /* ECUC_DEV_ERROR_DETECT == STD_ON */

/** \brief SID of service EcuC_SetMetaDataAddrExtension() */
#define ECUC_SID_ECUC_SETMETADATAADDREXTENSION        0x01U
/** \brief SID of service EcuC_GetMetaDataAddrExtension() */
#define ECUC_SID_ECUC_GETMETADATAADDREXTENSION        0x02U
/** \brief SID of service EcuC_SetMetaDataCanId() */
#define ECUC_SID_ECUC_SETMETADATACANID                0x03U
/** \brief SID of service EcuC_GetMetaDataCanId() */
#define ECUC_SID_ECUC_GETMETADATACANID                0x04U
/** \brief SID of service EcuC_SetMetaDataEthMac() */
#define ECUC_SID_ECUC_SETMETADATAETHMAC               0x05U
/** \brief SID of service EcuC_GetMetaDataEthMac() */
#define ECUC_SID_ECUC_GETMETADATAETHMAC               0x06U
/** \brief SID of service EcuC_SetMetaDataLinNad() */
#define ECUC_SID_ECUC_SETMETADATALINNAD               0x07U
/** \brief SID of service EcuC_GetMetaDataLinNad() */
#define ECUC_SID_ECUC_GETMETADATALINNAD               0x08U
/** \brief SID of service EcuC_SetMetaDataPriority() */
#define ECUC_SID_ECUC_SETMETADATAPRIORITY             0x09U
/** \brief SID of service EcuC_GetMetaDataPriority() */
#define ECUC_SID_ECUC_GETMETADATAPRIORITY             0x0AU
/** \brief SID of service EcuC_SetMetaDataSoConId() */
#define ECUC_SID_ECUC_SETMETADATASOCONID              0x0BU
/** \brief SID of service EcuC_GetMetaDataSoConId() */
#define ECUC_SID_ECUC_GETMETADATASOCONID              0x0CU
/** \brief SID of service EcuC_SetMetaDataSourceAddr() */
#define ECUC_SID_ECUC_SETMETADATASOURCEADDR           0x0DU
/** \brief SID of service EcuC_GetMetaDataSourceAddr() */
#define ECUC_SID_ECUC_GETMETADATASOURCEADDR           0x0EU
/** \brief SID of service EcuC_SetMetaDataTargetAddr() */
#define ECUC_SID_ECUC_SETMETADATATARGETADDR           0x0FU
/** \brief SID of service EcuC_GetMetaDataTargetAddr() */
#define ECUC_SID_ECUC_GETMETADATATARGETADDR           0x10U
/** \brief SID of service EcuC_SetMetaDataPayloadType() */
#define ECUC_SID_ECUC_SETMETADATAPAYLOADTYPE          0x11U
/** \brief SID of service EcuC_GetMetaDataPayloadType() */
#define ECUC_SID_ECUC_GETMETADATAPAYLOADTYPE          0x12U

/** \brief Det error Id ECUC_E_INV_ARG */
#define ECUC_E_INV_ARG                                0x01U

/** \brief Det error Id ECUC_E_PARAM_POINTER */
#define ECUC_E_PARAM_POINTER                          0x02U

/* Length of MAC address in bytes */
#define ECUC_MAC_ADDRESS_LEN                             6U

#if ECUC_META_DATA_HANDLING_ENABLED == STD_ON
/* Configuration of ComStack_Helper.h macros for the EcuC. */
#define ComStack_Uint8PtrType EcuC_Uint8PtrType

#define ComStack_ConstUint8PtrType EcuC_ConstUint8PtrType

/*==================[type definitions]=======================================*/

/** \brief The data pointer used for all EcuC 'octet' data streams */
typedef P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) EcuC_Uint8PtrType;

/** \brief The data pointer used for all const 'octet' EcuC data streams */
typedef P2CONST(uint8, AUTOMATIC, ECUC_APPL_DATA) EcuC_ConstUint8PtrType;

#endif /* ECUC_META_DATA_HANDLING_ENABLED == STD_ON */

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

#define ECUC_START_SEC_CODE
#include <EcuC_MemMap.h>

#if (ECUC_META_DATA_HANDLING_ENABLED == STD_ON)

/** \brief get meta data uint8
 *
 * This function gets uint8 values from meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[out] PduInfoPtr     pointer to the PDU info including
 *                            the pointer to the meta data
 * \param[out] MetaDataItem   pointer to the requested meta data item
 *
 * \return Result of operation
 * \retval E_OK               value has been copied to MetaDataItem
 * \retval E_NOT_OK           requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint8
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
);

/** \brief get meta data uint16
 *
 * This function gets uint8 values from meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[out]  PduInfoPtr    pointer to the PDU info including
 *                            the pointer to the meta data
 * \param[out] MetaDataItem   pointer to the requested meta data item
 *
 * \return Result of operation
 * \retval E_OK               value has been copied to MetaDataItem
 * \retval E_NOT_OK           requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint16
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
);

/** \brief get meta data uint32
 *
 * This function gets uint8 values from meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[out]  PduInfoPtr    pointer to the PDU info including
 *                            the pointer to the meta data
 * \param[out] MetaDataItem   pointer to the requested meta data item
 *
 * \return Result of operation
 * \retval E_OK               value has been copied to MetaDataItem
 * \retval E_NOT_OK           requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint32
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
);


/** \brief get pointer to meta data
 *
 * This function gets a pointer pointing to the requested meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[out]  PduInfoPtr    pointer to the PDU info including
 *                            the pointer to the meta data
 * \param[out] MetaDataItem   pointer to the requested meta data item
 * \param[in] Length          meta data item length
 *
 * \return Result of operation
 * \retval E_OK               value has been copied to MetaDataItem
 * \retval E_NOT_OK           requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPtr
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem,
  uint8 Length
);


/** \brief set meta data uint8
 *
 * This function sets uint8 values to meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[in] Value           value to set
 *
 * \return Result of operation
 * \returnValue E_OK             value has been saved
 * \returnValue E_NOT_OK         requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint8
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint8 Value
);

/** \brief set meta data uint16
 *
 * This function sets uint16 values to meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[in] Value           value to set
 *
 * \return Result of operation
 * \returnValue E_OK             value has been saved
 * \returnValue E_NOT_OK         requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint16
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint16 Value
);


/** \brief set meta data uint32
 *
 * This function sets uint32 values to meta data item
 *
 * \param[in] PduId           ID of the PDU
 * \param[in] MetaDataItemId  ID of the MetaDataItem
 * \param[in] Value           value to set
 *
 * \return Result of operation
 * \returnValue E_OK             value has been saved
 * \returnValue E_NOT_OK         requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint32
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint32 Value
);


/** \brief set meta data pointer
 *
 * This function sets the pointer to meta data item
 *
 * \param[in] PduId              ID of the PDU
 * \param[in] MetaDataItemId     ID of the MetaDataItem
 * \param[in] ValuePtr           value pointer
 * \param[in] Length             value length
 *
 * \return Result of operation
 * \returnValue E_OK             value has been saved
 * \returnValue E_NOT_OK         requested meta data item not configured for this PDU
 */
STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPtr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(uint8, AUTOMATIC, ECUC_APPL_DATA) ValuePtr,
  uint8 Length
);

#endif /* (ECUC_META_DATA_HANDLING_ENABLED == STD_ON) */

#define ECUC_STOP_SEC_CODE
#include <EcuC_MemMap.h>

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#define ECUC_START_SEC_CODE
#include <EcuC_MemMap.h>

#if (ECUC_META_DATA_HANDLING_ENABLED == STD_ON)

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataAddrExtension,1 */
FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataAddrExtension
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 AddrExtension
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataUint8(PduId, PduInfoPtr, ECUC_METADATA_ITEM_ADDRESS_EXTENSION_8, AddrExtension);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATAADDREXTENSION, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataCanId,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataCanId
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint32 CanId
)
{
  Std_ReturnType retval = EcuC_SetMetaDataUint32(PduId, PduInfoPtr, ECUC_METADATA_ITEM_CAN_ID_32, CanId);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATACANID, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataEthMac,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataEthMac
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2CONST(uint8, AUTOMATIC, ECUC_APPL_DATA) EthMacPtr
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataPtr (PduId, PduInfoPtr, ECUC_METADATA_ITEM_ETHERNET_MAC_64, EthMacPtr, ECUC_MAC_ADDRESS_LEN);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATAETHMAC, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataLinNad,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataLinNad
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 LinNad
)
{
  Std_ReturnType retval = EcuC_SetMetaDataUint8(PduId, PduInfoPtr, ECUC_METADATA_ITEM_LIN_NAD_8, LinNad);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATALINNAD, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataPriority,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPriority
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint8 Priority
)
{
  Std_ReturnType retval = EcuC_SetMetaDataUint8(PduId, PduInfoPtr, ECUC_METADATA_ITEM_PRIORITY_8, Priority);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATAPRIORITY, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataSoConId,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataSoConId
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 SoConId
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataUint16(PduId, PduInfoPtr, ECUC_METADATA_ITEM_SOCKET_CONNECTION_ID_16, SoConId);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATASOCONID, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataSourceAddr,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataSourceAddr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 SrcAddr
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataUint16(PduId, PduInfoPtr, ECUC_METADATA_ITEM_SOURCE_ADDRESS_16, SrcAddr);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATASOURCEADDR, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataTargetAddr,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataTargetAddr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 TargetAddr
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataUint16(PduId, PduInfoPtr, ECUC_METADATA_ITEM_TARGET_ADDRESS_16, TargetAddr);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATATARGETADDR, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataAddrExtension,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataAddrExtension
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) AddrExtension
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(AddrExtension == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAADDREXTENSION, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {
    retval =
      EcuC_GetMetaDataUint8(PduId, ECUC_METADATA_ITEM_ADDRESS_EXTENSION_8, PduInfoPtr, AddrExtension);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAADDREXTENSION, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataCanId,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataCanId
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, ECUC_APPL_DATA) CanId
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(CanId == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATACANID, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {

    retval = EcuC_GetMetaDataUint32(PduId, ECUC_METADATA_ITEM_CAN_ID_32, PduInfoPtr, CanId);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATACANID, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataEthMac,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataEthMac
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) EthMacPtr
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(EthMacPtr == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAETHMAC, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {
    retval =
      EcuC_GetMetaDataPtr(PduId, ECUC_METADATA_ITEM_ETHERNET_MAC_64, PduInfoPtr, EthMacPtr, ECUC_MAC_ADDRESS_LEN);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAETHMAC, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataLinNad,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataLinNad
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) LinNad
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(LinNad == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATALINNAD, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {

    retval = EcuC_GetMetaDataUint8(PduId, ECUC_METADATA_ITEM_LIN_NAD_8, PduInfoPtr, LinNad);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATALINNAD, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataPriority,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPriority
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) Priority
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(Priority == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAPRIORITY, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {

    retval = EcuC_GetMetaDataUint8(PduId, ECUC_METADATA_ITEM_PRIORITY_8, PduInfoPtr, Priority);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAPRIORITY, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataSoConId,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataSoConId
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) SoConId
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(SoConId == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATASOCONID, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {
    retval =
      EcuC_GetMetaDataUint16(PduId, ECUC_METADATA_ITEM_SOCKET_CONNECTION_ID_16, PduInfoPtr, SoConId);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATASOCONID, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataSourceAddr,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataSourceAddr
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) SrcAddr
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(SrcAddr == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATASOURCEADDR, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {
    retval =
      EcuC_GetMetaDataUint16(PduId, ECUC_METADATA_ITEM_SOURCE_ADDRESS_16, PduInfoPtr, SrcAddr);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATASOURCEADDR, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataTargetAddr,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataTargetAddr
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) TargetAddr
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(TargetAddr == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATATARGETADDR, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {

    retval =
      EcuC_GetMetaDataUint16(PduId, ECUC_METADATA_ITEM_TARGET_ADDRESS_16, PduInfoPtr, TargetAddr);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATATARGETADDR, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_SetMetaDataPayloadType,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPayloadType
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  uint16 PldType
)
{
  Std_ReturnType retval =
    EcuC_SetMetaDataUint16(PduId, PduInfoPtr, ECUC_METADATA_ITEM_PAYLOAD_TYPE_16, PldType);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(retval == E_NOT_OK)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_SETMETADATAPAYLOADTYPE, ECUC_E_INV_ARG);
  }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */

  return retval;
}

/* !LINKSTO EcuC.ArchDesign.MetaData.API.EcuC_GetMetaDataPayloadType,1 */
FUNC (Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPayloadType
(
  EcuC_PduIdType PduId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) PldType
)
{
  Std_ReturnType retval = E_NOT_OK;

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
  if(PldType == NULL_PTR)
  {
    ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAPAYLOADTYPE, ECUC_E_PARAM_POINTER);
  }
  else
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  {

    retval =
      EcuC_GetMetaDataUint16(PduId, ECUC_METADATA_ITEM_PAYLOAD_TYPE_16, PduInfoPtr, PldType);

#if (ECUC_DEV_ERROR_DETECT == STD_ON)
    if(retval == E_NOT_OK)
    {
      ECUC_REPORT_ERROR(ECUC_SID_ECUC_GETMETADATAPAYLOADTYPE, ECUC_E_INV_ARG);
    }
#endif /* (ECUC_DEV_ERROR_DETECT == STD_ON) */
  }
  return retval;
}

#endif /* (ECUC_META_DATA_HANDLING_ENABLED == STD_ON) */

/*==================[internal function definitions]==========================*/

#if (ECUC_META_DATA_HANDLING_ENABLED == STD_ON)

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint8
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 dataOffset;
  uint16 dataListNum;
  uint32 i;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++ )
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        *MetaDataItem = *EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr;
        retval = E_OK;
        break;
      }
    }
  }

  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint16
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint16, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 i;
  uint32 dataOffset;
  uint16 dataListNum;

  TS_PARAM_UNUSED(PduInfoPtr);
  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++ )
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {

        *MetaDataItem =
          COMSTACK_GET16(EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr, 0);
        retval = E_OK;
        break;
      }
    }
  }
  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataUint32
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint32, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 i;
  uint32 dataOffset;
  uint16 dataListNum;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++ )
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        *MetaDataItem =
          COMSTACK_GET32(EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr, 0);
        retval = E_OK;
        break;
      }
    }
  }
  return retval;
}


STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_GetMetaDataPtr
(
  EcuC_PduIdType PduId,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) MetaDataItem,
  uint8 Length
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 i;
  uint32 dataOffset;
  uint16 dataListNum;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++ )
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        TS_MemCpy( MetaDataItem,
                   EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr,
                   Length
                 );
        retval = E_OK;
        break;
      }
    }
  }
  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint8
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint8 Value
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 dataOffset;
  uint16 dataListNum;
  uint32 i;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++)
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        *EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr = Value;
        retval = E_OK;
        break;
      }
    }
  }

  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint16
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint16 Value
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 dataOffset;
  uint16 dataListNum;
  uint32 i;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++)
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        COMSTACK_SET16(EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr, 0, Value);
        retval = E_OK;
        break;
      }
    }
  }

  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataUint32
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  uint32 Value
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 dataOffset;
  uint16 dataListNum;
  uint32 i;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i<(dataOffset + dataListNum); i++)
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        COMSTACK_SET32(EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr, 0, Value);
        retval = E_OK;
        break;
      }
    }
  }
  return retval;
}

STATIC FUNC(Std_ReturnType, ECUC_CODE) EcuC_SetMetaDataPtr
(
  EcuC_PduIdType PduId,
  P2VAR(PduInfoType, AUTOMATIC, ECUC_APPL_DATA) PduInfoPtr,
  EcuC_MetaDataItemIdType MetaDataItemId,
  P2CONST(uint8, AUTOMATIC, ECUC_APPL_DATA) ValuePtr,
  uint8 Length
)
{
  Std_ReturnType retval = E_NOT_OK;
  uint32 dataOffset;
  uint16 dataListNum;
  uint32 i;

  TS_PARAM_UNUSED(PduInfoPtr);

  if(PduId < EcuC_ConfigData.numConfigPdus)
  {
    dataOffset = EcuC_ConfigData.pduConfig[PduId].metaDataItemListOffset;
    dataListNum = EcuC_ConfigData.pduConfig[PduId].metaDataItemListNum;

    for ( i=dataOffset; i< (dataOffset + dataListNum); i++)
    {
      if(MetaDataItemId == EcuC_ConfigData.metaDataItemConfig[i].metaDataItemId)
      {
        TS_MemCpy( EcuC_ConfigData.metaDataItemConfig[i].metaDataItem.metaDataItemOffsetPtr,
                   ValuePtr,
                   Length
                 );
        retval = E_OK;
        break;
      }
    }
  }

  return retval;
}

#endif /* (ECUC_META_DATA_HANDLING_ENABLED == STD_ON) */

#define ECUC_STOP_SEC_CODE
#include <EcuC_MemMap.h>

/*==================[end of file]============================================*/

