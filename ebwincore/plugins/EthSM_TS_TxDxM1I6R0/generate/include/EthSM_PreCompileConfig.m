[!/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHSM_PRECOMPILECONFIG_M'))"!][!//
[!VAR "ETHSM_PRECOMPILECONFIG_M"="'true'"!][!/*

*/!]  [!VAR "relocatable" = "'false'"!][!//
      [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthSM')) = 'true'"!][!//
      [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!][!//
      [!VAR "relocatable" = "'true'"!][!//
      [!ENDIF!][!//
      [!ELSE!][!//
      [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMRelocatablePbcfgEnable = 'true'"!][!//
      [!VAR "relocatable" = "'true'"!][!//
      [!ENDIF!][!//
      [!ENDIF!][!//

      <parameter name="relocatable-enable">
        <boolean-value>[!"$relocatable"!]</boolean-value>
      </parameter>

      <type name="EthSM_ConfigType">
        <comment><![CDATA[ Type definition of the root structure.]]></comment>
        <struct>
          <!-- Pointers to post build data structures -->
          <member name="PlatformSignature" type="uint32" comment="Used to validate the platform."/>
          <member name="LcfgSignature" type="uint32" comment="Used to validate the post build configuration against the link time configuration."/>
          <member name="CfgSignature" type="uint32" comment="Used to validate the post build configuration against the precompile time configuration."/>
          <member name="PublicInfoSignature" type="uint32" comment="Used to validate Public information."/>
          <member name="NetworkRef" type="EthSM_NetworkType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="NumNetworks" type="uint8">
          </member>
        </struct>
      </type>

      <type name="EthSM_NetworkType">
        <comment><![CDATA[ Type of network configuration structure.]]></comment>
        <struct>
          <member name="CtrlIdx" type="uint8">
          </member>
          <member name="NetworkHandleIdx" type="uint8">
          </member>
          [!IF "as:modconf('EthSM')[1]/EthSMGeneral/EthSMDevAuthSupport = 'true'"!]
          <member name="DevAuthNotifEnable" type="boolean">
          </member>
          [!ENDIF!]
          [!IF "as:modconf('EthSM')[1]/ReportToDem/EthSMDemCtrlTestResultReportToDem = 'DEM'"!]
          <member name="DemEventIdx" type="uint8">
          </member>
          [!ENDIF!]
        </struct>
      </type>

      <!-- Invalid value used for Dem ID (0 is not a valid value for Dem_EventIdType) -->
      <symbol-def name="ETHSM_INVALID_DEM_EVENTID">
        <integer-value>0</integer-value>
      </symbol-def>

[!ENDIF!][!// ETHSM_PRECOMPILECONFIG_M
