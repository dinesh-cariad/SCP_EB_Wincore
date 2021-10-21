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

/*==================[inclusions]============================================*/

#include <EcuC_Cfg.h>

[!AUTOSPACING!]

[!SELECT "as:modconf('EcuC')[1]"!]
[!INCLUDE "../include/EcuC_Vars.m"!]
[!INCLUDE "../include/CommonMacros.m"!]

/*==================[macros]================================================*/

[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucMetaDataHandlingEnabled = 'true'"!]

[!//Getting Meta Data Id from an item
[!MACRO "Get_Meta_Data_Item_Id", "ItemType"!]
  [!VAR "value"="'null'"!]
  [!IF "$ItemType = 'ETHERNET_MAC_64'"!]
    [!VAR "value" = "num:i(0)"!]
  [!ELSEIF "$ItemType = 'CAN_ID_32'"!]
    [!VAR "value" = "num:i(1)"!]
  [!ELSEIF "$ItemType = 'SOCKET_CONNECTION_ID_16'"!]
    [!VAR "value" = "num:i(2)"!]
  [!ELSEIF "$ItemType = 'SOURCE_ADDRESS_16'"!]
    [!VAR "value" = "num:i(3)"!]
  [!ELSEIF "$ItemType = 'TARGET_ADDRESS_16'"!]
    [!VAR "value" = "num:i(4)"!]
  [!ELSEIF "$ItemType = 'ADDRESS_EXTENSION_8'"!]
    [!VAR "value" = "num:i(5)"!]
  [!ELSEIF "$ItemType = 'LIN_NAD_8'"!]
    [!VAR "value" = "num:i(6)"!]
  [!ELSEIF "$ItemType = 'PRIORITY_8'"!]
    [!VAR "value" = "num:i(7)"!]
  [!ELSEIF "$ItemType = 'PAYLOAD_TYPE_16'"!]
    [!VAR "value" = "num:i(8)"!]
  [!ENDIF!]
            [!"$value"!]U, /* metaDataItemId */
[!ENDMACRO!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define ECUC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <EcuC_MemMap.h>

/* Space reserved for configured meta data items
 * Memory is aligned to the most stringent alignment requirement
 * of any simple data type available on the respective platform,
 * e.g. uint32.*/

#if (ECUC_META_DATA_CONFIG_SIZE_MAX_64 > 0) /* To prevent empty arrays */
static TS_DefMaxAlignedByteArray(EcuC_MetaDataMemory, ECUC, VAR_CLEARED, (ECUC_META_DATA_CONFIG_SIZE_MAX_64 * 8U));
#endif /* (ECUC_META_DATA_CONFIG_SIZE_MAX_64 > 0) */

#define ECUC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <EcuC_MemMap.h>

/*==================[external constants]====================================*/
#define ECUC_START_SEC_CONST_UNSPECIFIED
#include <EcuC_MemMap.h>

/* Global configuration data */
CONST(EcuC_ConfigType, ECUC_CONST) EcuC_ConfigData =
{
  /* pduConfig */
  {
    [!VAR "Meta_Data_Item_Offset" = "num:i(0)"!]
    [!LOOP "node:order(node:filter(as:modconf("EcuC")[1]/EcucPduCollection/Pdu/*,'node:exists(PduId)'), 'PduId' )"!][!//
     [!VAR "PduIdent"="PduId"!]
     { /* [!"concat('pduConfig[',$PduIdent,']',' -- EcuCConf_PduId_',name(.))"!] */
        [!IF "node:refvalid(./MetaDataTypeRef)"!]
          [!VAR "Meta_Data_Items" = "num:i(0)"!]
          [!VAR "Meta_Data_Items" = "num:i(count(node:ref(./MetaDataTypeRef)/MetaDataItem/*))"!]
          [!VAR "Meta_Data_Item_Offset_Internal" = "num:i(0)"!]
          [!FOR "z" = "1" TO "$EcuC_Meta_Data_Cache_Size"!]
             [!IF "node:path(node:ref(./MetaDataTypeRef)) = node:path(as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*[num:i($z)]) "!]
               [!VAR "Meta_Data_Item_Offset" = "$Meta_Data_Item_Offset_Internal"!]
               [!BREAK!]
             [!ENDIF!]
             [!VAR "Meta_Data_Item_Offset_Internal" = "num:i($Meta_Data_Item_Offset_Internal) + num:i(count(as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*[num:i($z)]/MetaDataItem/*))"!]
          [!ENDFOR!]
          [!"num:i($Meta_Data_Item_Offset)"!]U, /* metaDataItemListOffset */
          [!"$Meta_Data_Items"!]U /* metaDataItemListNum */
        [!ELSE!]
          [!"num:i(0)"!]U, /* metaDataItemListOffset */
          [!"num:i(0)"!]U /* metaDataItemListNum */
        [!ENDIF!]
      },
    [!ENDLOOP!]
  },
  /* metaDataItemConfig */
  {
    [!VAR "Meta_Data_Item_Array_Position" = "num:i(0)"!]
    [!VAR "Meta_Data_Item_Offset_Internal" = "num:i(0)"!]
    [!LOOP "as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*"!]
      [!//Go through all Items and check which are in use, save their length
      [!LOOP "./MetaDataItem/*"!]
        [!VAR "Meta_Data_Item_Type" = "./MetaDataItemType"!]
        [!VAR "Meta_Data_Item_Length" = "./MetaDataItemLength"!]
        {  /* [!"concat('metaDataItemConfig[',num:i($Meta_Data_Item_Array_Position),']',' -- ',name(../..))"!] */ 
          [!CALL "Get_Meta_Data_Item_Id", "ItemType"="$Meta_Data_Item_Type"!]
          {
            (&((P2VAR(uint8, AUTOMATIC, ECUC_APPL_DATA))EcuC_MetaDataMemory)[[!"num:i($Meta_Data_Item_Offset_Internal)"!]U]), /* metaDataItemOffsetPtr */
            [!"$Meta_Data_Item_Length"!]U /* metaDataLength for [!"$Meta_Data_Item_Type"!] */
          }
        },
        [!VAR "Meta_Data_Item_Array_Position" = "num:i($Meta_Data_Item_Array_Position) + num:i(1)"!]
        [!VAR "Meta_Data_Item_Offset_Internal" = "$Meta_Data_Item_Offset_Internal + num:i($Meta_Data_Item_Length)"!]
      [!ENDLOOP!]
      [!VAR "remainder" = "num:i($Meta_Data_Item_Offset_Internal mod 8)"!]
      [!IF "$remainder > num:i(0)"!]
        [!VAR "Meta_Data_Item_Offset_Internal" = "$Meta_Data_Item_Offset_Internal + num:i(8) - $remainder"!]
      [!ENDIF!]
    [!ENDLOOP!]
  },
  [!"num:i($EcuC_PduId_Cache_Size)"!]U /* numConfigPdus */
};

#define ECUC_STOP_SEC_CONST_UNSPECIFIED
#include <EcuC_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

[!ENDIF!]
[!ENDSELECT!]
