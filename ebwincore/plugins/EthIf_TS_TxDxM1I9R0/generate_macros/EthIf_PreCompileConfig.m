[!/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHIF_PRECOMPILECONFIG_M'))"!][!//
[!VAR "ETHIF_PRECOMPILECONFIG_M"="'true'"!][!/*

*/!]  [!VAR "relocatable" = "'false'"!][!//
      [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('EthIf')) = 'true'"!][!//
      [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!][!//
      [!VAR "relocatable" = "'true'"!][!//
      [!ENDIF!][!//
      [!ELSE!][!//
      [!IF "as:modconf('EthIf')[1]/EthIfGeneral/EthIfRelocatablePbcfgEnable = 'true'"!][!//
      [!VAR "relocatable" = "'true'"!][!//
      [!ENDIF!][!//
      [!ENDIF!][!//

      <parameter name="relocatable-enable">
        <boolean-value>[!"$relocatable"!]</boolean-value>
      </parameter>

      <!-- !LINKSTO EthIf.ASR43.SWS_EthIf_00149,1 -->
      <type name="EthIf_ConfigType">
        <comment><![CDATA[ Type definition of the root structure.]]></comment>
        <struct>
          <!-- Pointers to post build data structures -->
          <member name="PlatformSignature" type="uint32" comment="Used to validate the platform."/>
          <member name="LcfgSignature" type="uint32" comment="Used to validate the post build configuration against the link time configuration."/>
          <member name="CfgSignature" type="uint32" comment="Used to validate the post build configuration against the precompile time configuration."/>
          <member name="PublicInfoSignature" type="uint32" comment="Used to validate Public information."/>
          [!IF "$EthIf_TrcvSupportEnable = 'true'"!]
          <member name="EthIf_EthIfTrcvIdxRef" type="uint8">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_EthIfTrcvEthTrcvIdxMapRef" type="EthIf_TrcvType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_EthIfTrcvEthCtrlIdxMapRef" type="uint8">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          [!ENDIF!]
          <member name="EthIf_EthIfCtrlRef" type="EthIf_CtrlType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_PhysCtrlRef" type="EthIf_PhysCtrlType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_EthIfSwtRef" type="uint8">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          [!IF "$EthIf_PortStateMgmtEnabled = 'true'"!]
          <member name="EthIf_SwtPortRef" type="EthIf_SwtPortType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_SwtPortGroupRef" type="EthIf_SwtPortGroupType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_PortsInPortGroupRef" type="uint8">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          [!ENDIF!]
          [!IF "(($EthIf_EthSupportApi = 'ASR430') or ($EthIf_EthSupportApi = 'ASR430_EB'))"!]
          <member name="EthIf_PhysCtrlEthIfCtrlMapDataRef" type="EthIf_PhysCtrlEthIfCtrlMapDataType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_MapPhysCtrlEthIfCtrlIdxRef" type="uint8">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="EthIf_EthIfPhysCtrlIngrFifoRef" type="EthIf_PhysCtrlIngrFifoType">
            <compiler-abstraction>
              <ref2cfg/>
            </compiler-abstraction>
          </member>
          <member name="NumMappedPrioIngrFifo" type="uint8">
          </member>
          <member name="NumIngrFifoTotal" type="uint8">
          </member>
          [!ENDIF!]
          <member name="PhyCtrlMax" type="uint8">
          </member>
          <member name="EthIfCtrlMax" type="uint8">
          </member>
          <member name="EthIfSwtMax" type="uint8">
          </member>
          <member name="EthIfTrcvMax" type="uint8">
          </member>
          [!IF "$EthIf_PortStateMgmtEnabled = 'true'"!]
          <member name="SwtPortGroupsMax" type="uint8">
          </member>
          <member name="SwtPortsMax" type="uint8">
          </member>
          [!ENDIF!]
        </struct>
      </type>

      <type name="EthIf_CtrlType">
        <comment><![CDATA[ Type of EthIf controller configuration structure.]]></comment>
        <struct>
          [!IF "$EthIf_VlanSupportEnable = 'true'"!]
          <member name="VID" type="uint16">
          </member>
          [!ENDIF!]
          <member name="PhysCtrlIdx" type="uint8">
          </member>
          [!IF "$EthIf_TrcvSupportEnable = 'true'"!]
          <member name="EthIfTrcvIdx" type="uint8">
          </member>
          [!IF "$EthIf_MiiApiEnable = 'true'"!]
          <member name="EthTrcvMiiIdx" type="uint8">
          </member>
          [!ENDIF!]
          [!ENDIF!]
          <member name="EthIfPortGroupIdx" type="uint8">
          </member>
        </struct>
      </type>

      <type name="EthIf_PhysCtrlType">
        <comment><![CDATA[ Type of Phys controller configuration structure.]]></comment>
        <struct>
          <member name="EthCtrlIdx" type="uint8">
          </member>
          [!IF "$EthIf_CtrlBswRefNr > 1"!]
          <member name="EthCtrlApiIdx" type="uint8">
          </member>
          [!ENDIF!]
        </struct>
      </type>
      <type name="EthIf_TrcvType">
        <comment><![CDATA[ Type of Transceiver configuration structure.]]></comment>
        <struct>
          <member name="EthTrcvIdx" type="uint8">
          </member>
          [!IF "$EthIf_TrcvBswRefNr > 1"!]
          <member name="EthTrcvApiIdx" type="uint8">
          </member>
          [!ENDIF!]
        </struct>
      </type>
      <type name="EthIf_SwtPortType">
        <comment><![CDATA[ Type of Switch port configuration structure.]]></comment>
        <struct>
          <member name="EthSwtIdx" type="uint8">
          </member>
          <member name="EthSwtPortIdx" type="uint8">
          </member>
          [!IF "$EthIf_PortStateMgmtEnabled = 'true'"!]
          <member name="EthSwtPortRole" type="EthIf_PortRoleType">
          </member>
          <member name="EthTrcvIdx" type="uint8">
          </member>
          [!ENDIF!]
        </struct>
      </type>

      <!-- Constant for a port role STANDARD - port is neither a host nor an uplink port -->
      <symbol-def name="ETHIF_PORTROLE_STANDARD">
        <integer-value>0</integer-value>
      </symbol-def>

      <!-- Constant for a port role UPLINK - port is an uplink port -->
      <symbol-def name="ETHIF_PORTROLE_UPLINK">
        <integer-value>1</integer-value>
      </symbol-def>

      <!-- Constant for a port role HOST - port is a host port -->
      <symbol-def name="ETHIF_PORTROLE_HOST">
        <integer-value>2</integer-value>
      </symbol-def>

      <type name="EthIf_PortRoleType">
        <comment><![CDATA[ Type for Ethernet switch port role: [PORTROLE_STANDARD, PORTROLE_UPLINK, PORTROLE_HOST].]]></comment>
        <reference type="uint8" />
      </type>

      <!-- This macro marks an invalid reference to a EthTrcv in EthIf configuration -->
      <symbol-def name="ETHIF_INVALID_TRCV_IDX">
        <integer-value>255</integer-value>
      </symbol-def>

      <!-- This macro marks an invalid reference to a EthTrcv in EthIf configuration for MIIidx -->
      <symbol-def name="ETHIF_INVALID_TRCV_MIIIDX">
        <integer-value>255</integer-value>
      </symbol-def>

      <!-- This macro marks an invalid reference to a EthIfSwitchRefOrPortGroupRef in EthIf configuration -->
      <symbol-def name="ETHIF_INVALID_PORTGROUP_IDX">
        <integer-value>255</integer-value>
      </symbol-def>

      <!-- This macro marks an invalid reference to a EthSwt in EthIf configuration -->
      <symbol-def name="ETHIF_INVALID_SWT_IDX">
        <integer-value>255</integer-value>
      </symbol-def>

     [!IF "$EthIf_VlanSupportEnable = 'true'"!]
      <!-- Invalid VID value (> 12 bits) to mark an non VLAN EthIf controller in configuration. -->
      <symbol-def name="ETHIF_VLAN_VID_UNUSED">
        <integer-value>4096</integer-value> <!-- 0x1000 -->
      </symbol-def>
     [!ENDIF!]

      <!-- Invalid DEM event id, indicating no DEM error is reported for this transceiver/port -->
      <symbol-def name="ETHIF_INVALID_DEM_EVENT_ID">
        <integer-value>0</integer-value>
      </symbol-def>

      <type name="EthIf_SwtPortGroupType">
        <comment><![CDATA[ Type of Switch port group configuration structure.]]></comment>
        <struct>
          <member name="BeginIdx" type="uint16">
          </member>
          <member name="NumPorts" type="uint8">
          </member>
          <member name="RefType" type="EthIf_PortGroupRefSemanticType">
          </member>
        </struct>
      </type>

      <!-- Constant for a reference type of a port group not referenced by any EthIfCtrl. Port group is under control of BswM. -->
      <symbol-def name="ETHIF_REFTYPE_NO_REF">
        <integer-value>0</integer-value>
      </symbol-def>

      <!-- Constant for a port group referenced by EthIfCtrl, with reference type LINK-INFORMATION. Port group is referenced by EthIf, but EthIf does not control it. Only a link state change is reported to EthIf. -->
      <symbol-def name="ETHIF_REFTYPE_LINK_INFO">
        <integer-value>1</integer-value>
      </symbol-def>

      <!-- Constant for a port group referenced by EthIfCtrl, with a reference type CONTROL. -->
      <symbol-def name="ETHIF_REFTYPE_CONTROL">
        <integer-value>2</integer-value>
      </symbol-def>

      <!-- Constant for a logical port group that represents all ports of a switch. It is referenced by EthIfCtrl -->
      <symbol-def name="ETHIF_REFTYPE_SWITCH">
        <integer-value>3</integer-value>
      </symbol-def>

      <type name="EthIf_PortGroupRefSemanticType">
        <comment><![CDATA[ Type for Port group reference semantics: [REFTYPE_NO_REF, REFTYPE_LINK_INFO, REFTYPE_CONTROL, REFTYPE_SWITCH].]]></comment>
        <reference type="uint8" />
      </type>

      <type name="EthIf_PhysCtrlEthIfCtrlMapDataType">
        <comment><![CDATA[ Type for mapping array from EthIf to Eth physical controller.]]></comment>
        <struct>
          <member name="EthIfCtrlIdxOffset" type="uint8">
          </member>
          <member name="EthIfCtrlIdNumber" type="uint8">
          </member>
        </struct>
      </type>

      <type name="EthIf_PhysCtrlIngrFifoType">
        <comment><![CDATA[ Type of EthIf Physical controller Ingress Fifo configuration structure.]]></comment>
        <struct>
          <member name="EthIfCtrlRxIndicationIterations" type="uint16">
          </member>
          <member name="PhysCtrlIdx" type="uint8">
          </member>
          <member name="EthFifoIdx" type="uint8">
          </member>
        </struct>
      </type>

[!ENDIF!][!// ETHIF_PRECOMPILECONFIG_M
