[!/**
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
 */!][!//

[!NOCODE!]

[!SELECT "as:modconf('EcuC')[1]"!][!//
[!//

[!IF "as:modconf('EcuC')[1]/EcucGeneral/EcucMetaDataHandlingEnabled = 'true'"!][!//Only do it if Meta Data Handling is enabled
[!//Number of the Pdus
[!VAR "EcuC_PduId_Cache_Size" = "num:i(count(as:modconf('EcuC')[1]/EcucPduCollection/Pdu/*/PduId))"!]

[!//Number of the configured Meta Data Types
[!VAR "EcuC_Meta_Data_Cache_Size" = "num:i(count(as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*))"!]

[!//Initialization and declaration of the global variable Offset_Internal
[!VAR "Meta_Data_Item_Offset_Internal" = "num:i(0)"!]


[!//Maximum size of Meta Data Cache
[!VAR "Meta_Data_Config_Size_Max" = "num:i(0)"!]
[!LOOP "as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*"!][!//
    [!VAR "Meta_Data_Config_Size_Max" = "$Meta_Data_Config_Size_Max + sum(./MetaDataItem/*/MetaDataItemLength)"!]
    [!VAR "remainder" = "num:i($Meta_Data_Config_Size_Max mod 8)"!]
    [!IF "$remainder > num:i(0)"!][!//
      [!VAR "Meta_Data_Config_Size_Max" = "$Meta_Data_Config_Size_Max + num:i(8) - $remainder"!]
    [!ENDIF!]
[!ENDLOOP!]
[!VAR "Meta_Data_Config_Size_Max" = "num:i(num:div($Meta_Data_Config_Size_Max, num:i(8)))"!]


[!//Number of the Meta Data Items
[!VAR "EcuC_Meta_Data_Item_List_Size" = "num:i(count(as:modconf('EcuC')[1]/EcucPduCollection/MetaDataType/*/MetaDataItem/*))"!]


[!ENDIF!][!//Meta Data Handling is enabled

[!ENDSELECT!]

[!ENDNOCODE!]
