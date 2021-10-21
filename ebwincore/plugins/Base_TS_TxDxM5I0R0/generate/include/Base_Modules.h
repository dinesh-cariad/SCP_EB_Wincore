/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!AUTOSPACING!][!//
[!MACRO "GenerateConfigPtr","InModuleName", "InModuleApiPostfix", "InModuleConfigPath"!]
#ifndef BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_CONFIG_PTR
[!/* special handling for non post-build ComM */!][!//
  [!IF "($InModuleName = 'ComM') and ($implementationConfigVariant != 'VariantPostBuild')"!][!//
/** \brief empty for [!"$InModuleName"!] -> special handling for [!"$InModuleName"!] ([!"$implementationConfigVariant"!]) */
#define BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_CONFIG_PTR
  [!ELSE!][!//
[!/* check if the module shall be initialized by the PbcfgM */!][!//
    [!IF "count(as:modconf('PbcfgM')[1]/PbcfgMBswModules/*[node:refvalid(PbcfgMBswModuleRef) and (as:path(as:ref(PbcfgMBswModuleRef)) = $InModuleConfigPath)])>0"!][!//
/** \brief NULL_PTR for [!"$moduleName"!] ([!"$InModuleConfigPath"!]) -> initialized by PbcfgM */
#define BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_CONFIG_PTR[!WS!]NULL_PTR
    [!ELSE!][!//
/** \brief Address of the first multiple configuration container for [!"$moduleName"!] ([!"$InModuleConfigPath"!]) */
    [!IF "@type = 'MAP'"!]
      [!VAR "configShortName"="name(*[1])"!][!//
    [!ELSE!][!//
      [!VAR "configShortName"="as:name(.)"!][!//
    [!ENDIF!][!//
#define BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_CONFIG_PTR[!WS!](&[!"$configShortName"!])
    [!ENDIF!][!//
  [!ENDIF!][!//
#endif
[!ENDMACRO!]
[!MACRO "GenerateInclude","InModuleName", "InModuleApiPostfixForDefine", "InModuleApiPostfix", "InModuleConfigPath"!]
#ifndef BASE_[!"text:toupper($moduleName)"!][!"text:toupper($InModuleApiPostfixForDefine)"!]_HEADER
/** \brief Name of the [!"$moduleName"!] ([!"$InModuleConfigPath"!]) module's main header file */
#define BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfixForDefine)"!]_HEADER <[!"$InModuleName"!][!"$InModuleApiPostfix"!].h>
#endif
[!ENDMACRO!]
[!MACRO "GenerateEnable","InModuleName", "InModuleApiPostfix", "InModuleConfigPath", "InImplementationConfigVariant"!]
#ifndef BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_ENABLED
/** \brief Enable status of the module [!"$InModuleName"!] ([!"$InModuleConfigPath"!]) ([!"$InImplementationConfigVariant"!]) */
#define BASE_[!"text:toupper($InModuleName)"!][!"text:toupper($InModuleApiPostfix)"!]_ENABLED [!//
[!IF "node:containsValue(text:split($Base_EnabledModules),$InModuleName) or node:containsValue(text:split($Base_EnabledModules),concat($InModuleName,$InModuleApiPostfix))"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#endif
[!ENDMACRO!]
[!MACRO "GetModuleName","InModuleDef"!]
[!/* special handling for Xfrm and Cdd modules */!][!//
  [!VAR "moduleName"="text:split($InModuleDef, '/')[last()]"!][!//
  [!IF "($moduleName = 'Xfrm') or ($moduleName = 'Cdd')"!][!//
    [!VAR "moduleVariant"="text:split($InModuleDef, '/')[last()-1]"!][!//
    [!IF "contains($moduleVariant, '_TS_')"!][!//
      [!VAR "apiServicePrefix"="substring-before($moduleVariant, '_TS_')"!][!//
    [!ELSEIF "contains($moduleVariant, '_')"!][!//
      [!VAR "apiServicePrefix"="substring-before($moduleVariant, '_')"!][!//
    [!ELSE!][!//
      [!VAR "apiServicePrefix"="$moduleVariant"!][!//
    [!ENDIF!][!//
    [!IF "$apiServicePrefix != ''"!][!//
[!"$apiServicePrefix"!][!//
    [!ELSE!][!//
      [!ERROR!][!"$InModuleDef"!]: apiServicePrefix is missing[!ENDERROR!][!//
    [!ENDIF!][!//
  [!ELSEIF "contains($moduleName, '_TS_')"!][!//
[!"substring-before($moduleName, '_TS_')"!][!//
  [!ELSEIF "contains($moduleName, '_')"!][!//
[!"substring-before($moduleName, '_')"!][!//
  [!ELSE!][!//
[!"$moduleName"!][!//
  [!ENDIF!][!//
[!ENDMACRO!]
[!/* Get all enabled modules */!][!//
[!VAR "Base_EnabledModules"!][!//
[!LOOP "text:split($enabledModules)"!][!CALL "GetModuleName", "InModuleDef"="."!][!WS!][!ENDLOOP!][!//
[!ENDVAR!][!//

/* !LINKSTO Base.ModuleInfo.HeaderFile,1 */
#ifndef BASE_MODULES_H
#define BASE_MODULES_H

/*==================[macros]=================================================*/

[!/* Iterate over all module configurations, sorted by extracted module name */!][!//
[!LOOP "node:order(/AUTOSAR/TOP-LEVEL-PACKAGES/*/ELEMENTS/*[@type='MODULE-CONFIGURATION'], 'text:split(as:path(as:dtos(.)), &apos;/&apos;)[position()=last()]')"!]
[!/* tresos Studio does not support to directly get the name of the module
   * from the schema node, so we have to extract it from the last element of the
   * schema path */!][!//
  [!VAR "moduleName"!][!CALL "GetModuleName","InModuleDef"="as:path(as:dtos(.))"!][!ENDVAR!][!//
[!/* check if there is an implementationConfigVariant element. - If not fall back to
   * 'VariantPreCompile'. Note that there are modules that don't have this element
   * (e.g., MemMap) */!][!//
  [!IF "node:exists(./IMPLEMENTATION_CONFIG_VARIANT)"!]
    [!VAR "implementationConfigVariant" = "./IMPLEMENTATION_CONFIG_VARIANT"!]
  [!ELSE!][!//
    [!VAR "implementationConfigVariant" = "'VariantPreCompile'"!]
  [!ENDIF!]
  [!VAR "ModuleApiPostfix"="''"!]
  [!IF "node:exists(./CommonPublishedInformation/VendorApiInfix) and (./CommonPublishedInformation/VendorApiInfix != '')"!]
    [!VAR "ModuleApiPostfix"!]_[!"./CommonPublishedInformation/VendorId"!]_[!"./CommonPublishedInformation/VendorApiInfix"!][!ENDVAR!]
  [!ENDIF!]
  [!VAR "ModuleConfigPath"="as:path(.)"!]
  [!CALL "GenerateEnable","InModuleName"="$moduleName", "InModuleApiPostfix"="''", "InModuleConfigPath"="$ModuleConfigPath", "InImplementationConfigVariant"="$implementationConfigVariant"!]

  [!IF "$ModuleApiPostfix != ''"!]
    [!CALL "GenerateEnable","InModuleName"="$moduleName", "InModuleApiPostfix"="$ModuleApiPostfix", "InModuleConfigPath"="$ModuleConfigPath", "InImplementationConfigVariant"="$implementationConfigVariant"!]

  [!ENDIF!]
[!/* select the multiple configuration container of the module if there is any */!][!//
  [!SELECT "*[node:dtos(.)/@type = 'MULTIPLE-CONFIGURATION-CONTAINER']"!]

    [!IF "(count(*)!=1) and (@type = 'MAP')"!]
[!/* The multiplicity restriction of multiple config containers for
   * most (?) schema definitions should be warned during the code generation */!][!//
      [!WARNING!]
        Configuration container of module [!"$moduleName"!] contains more than one configuration set in [!"node:path(.)"!].
        Generating configuration pointer to first configuration; ensure that the init functions of the module
        are called with the correct combination of configuration pointers within the integration code.
      [!ENDWARNING!]
    [!ENDIF!]
    [!CALL "GenerateConfigPtr","InModuleName"="$moduleName", "InModuleApiPostfix"="''", "InModuleConfigPath"="$ModuleConfigPath"!]

    [!IF "$ModuleApiPostfix != ''"!]
      [!CALL "GenerateConfigPtr","InModuleName"="$moduleName", "InModuleApiPostfix"="$ModuleApiPostfix", "InModuleConfigPath"="$ModuleConfigPath"!]

    [!ENDIF!]
  [!ENDSELECT!]
[!/* special handling for PbcfgM regarding name of config (since this one does not have a multiple config container) */!][!//
  [!IF "$moduleName = 'PbcfgM'"!]
#ifndef BASE_PBCFGM_CONFIG_PTR
/** \brief Address of the post build configuration for PbcfgM */
#define BASE_PBCFGM_CONFIG_PTR (&(PbcfgM_Config.PbcfgM_RootConfig))
#endif

  [!ENDIF!]
  [!CALL "GenerateInclude","InModuleName"="$moduleName", "InModuleApiPostfixForDefine"="''", "InModuleApiPostfix"="$ModuleApiPostfix", "InModuleConfigPath"="$ModuleConfigPath"!]

  [!IF "$ModuleApiPostfix != ''"!]
    [!CALL "GenerateInclude","InModuleName"="$moduleName", "InModuleApiPostfixForDefine"="$ModuleApiPostfix", "InModuleApiPostfix"="$ModuleApiPostfix", "InModuleConfigPath"="$ModuleConfigPath"!]

  [!ENDIF!]
[!ENDLOOP!]
#endif /* ifndef BASE_MODULES_H */
/*==================[end of file]============================================*/
