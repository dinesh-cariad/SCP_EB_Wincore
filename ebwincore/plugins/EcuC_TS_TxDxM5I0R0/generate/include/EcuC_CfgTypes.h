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

[!SELECT "as:modconf('EcuC')[1]"!][!//
[!//

#ifndef ECUC_CFGTYPES_H
#define ECUC_CFGTYPES_H

[!AUTOSPACING!]

/*==================[inclusions]============================================*/
#include <TSAutosar.h>            /* basic AUTOSAR types */
#include <ComStack_Types.h>       /* Com Stack Types */

[!INCLUDE "../include/EcuC_Vars.m"!][!//
/*==================[macros]================================================*/

/*==================[defensive programming macros]==========================*/

/*==================[type definitions]======================================*/

[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucMetaDataHandlingEnabled = 'true'"!][!//Only do it if Meta Data Handling is enabled

typedef uint8 EcuC_MetaDataItemIdType;

/** \brief Identifies the id of the configured Pdu. */
typedef [!"text:tolower(as:modconf('EcuC')[1]/EcucPduCollection/PduIdTypeEnum)"!] EcuC_PduIdType;

typedef struct
{
  P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA) metaDataItemOffsetPtr;  /* start of meta data item in  EcuC_MetaDataMemory */
  uint8 metaDataLength;  /* and length of meta data item */
} EcuC_MetaDataItemType;

typedef struct
{
  uint32 metaDataItemListOffset; /* start of the meta data in metaDataItemConfig */
  uint16 metaDataItemListNum; /* number of configured meta data itmes for this pdu */
} EcuC_PduConfigType;

typedef struct
{
  EcuC_MetaDataItemIdType metaDataItemId; /* id of the meta data item */
  EcuC_MetaDataItemType metaDataItem; /* start of meta data item in  EcuC_MetaDataMemory, and length of meta data item */
} EcuC_MetaDataItemConfigType;

typedef struct
{
  EcuC_PduConfigType pduConfig[[!"num:i($EcuC_PduId_Cache_Size)"!]U]; /* list of configured pdus, from PduId 0 to maximal configured PduId */
  EcuC_MetaDataItemConfigType metaDataItemConfig[[!"num:i($EcuC_Meta_Data_Item_List_Size)"!]U]; /* list of configured meta data items, sorted according to meta data type index */
  uint16 numConfigPdus;
} EcuC_ConfigType;

[!ENDIF!][!//Meta Data Handling is enabled

/*==================[external data declarations]============================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[end of file]===========================================*/

#endif /* ECUC_CFGTYPES_H */
[!ENDSELECT!]
