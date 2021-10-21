[!/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2018 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!// multiple inclusion protection
[!IF "not(var:defined('ETHIF_PBCFG_M'))"!][!//
[!VAR "ETHIF_PBCFG_M"="'true'"!][!/*

=== PBCfg macros ===

*/!][!VAR "EthIf_EthNumMappedPrioFifo" = "0"!][!//
[!LOOP "node:order(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfPhysController/*,'EthIfPhysControllerIdx')"!][!//
  [!VAR "EthIf_EthNumMappedPrioFifo" = "$EthIf_EthNumMappedPrioFifo + count(./EthIfPhysCtrlRxMainFunctionPriorityProcessing/*)"!][!//
[!ENDLOOP!][!/*

*/!][!VAR "EthIf_EthNumIngressFifoTotal" = "0"!][!//
[!LOOP "node:order(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfPhysController/*,'EthIfPhysControllerIdx')"!][!//
  [!VAR "EthIf_EthNumIngressFifoTotal" = "$EthIf_EthNumIngressFifoTotal + count(node:ref(./EthIfEthCtrlRef)/EthCtrlConfigIngress/EthCtrlConfigIngressFifo/*)"!][!//
[!ENDLOOP!][!/*

  Macro to calculate EthIfPortIdx (port idx in the context of EthIf), out of SwitchIdx and PortIdx
  (in the context of EthSwt driver). EthIfPortIdx is used to reference EthIf_SwtPort[].
*/!][!MACRO "GetEthIfPortIdx", "SwtIdx" = "0", "SwtPortIdx" = "0"!][!/*
*/!][!VAR "PortPos" = "0"!][!/*
*/!][!LOOP "node:order(node:refs(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*/EthIfSwitchRef))"!][!/*
   */!][!IF "num:i(./EthSwtIdx)  < $SwtIdx"!][!/*
     */!][!VAR "PortPos" = "$PortPos + num:i(count(./EthSwtPort/*))"!][!/*
   */!][!ELSEIF "num:i(./EthSwtIdx) = ($SwtIdx)"!][!VAR "PortPos" = "$PortPos + $SwtPortIdx"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "CalcPortRole", "EthSwtPortRole" = ""!][!//
[!IF "$EthSwtPortRole = 'ETHSWT_HOST_PORT'"!][!//
              <field>
                <symbol>ETHIF_PORTROLE_HOST</symbol>
              </field>
[!ELSEIF "$EthSwtPortRole = 'ETHSWT_UP_LINK_PORT'"!][!//
              <field>
                <symbol>ETHIF_PORTROLE_UPLINK</symbol>
              </field>
[!ELSE!][!//
              <field>
                <symbol>ETHIF_PORTROLE_STANDARD</symbol>
              </field>
[!ENDIF!][!//
[!ENDMACRO!][!/*


*/!][!MACRO "CalcRefType", "RefSemantics" = ""!][!//
[!IF "$RefSemantics = 'ETHIF_SWITCH_PORT_GROUP_CONTROL'"!][!//
            <field>
              <symbol>ETHIF_REFTYPE_CONTROL</symbol>
            </field>
[!ELSEIF "$RefSemantics = 'ETHIF_SWITCH_PORT_GROUP_LINK_INFO'"!][!//
            <field>
              <symbol>ETHIF_REFTYPE_LINK_INFO</symbol>
            </field>
[!ELSE!][!//
            <field>
              <symbol>ETHIF_REFTYPE_NO_REF<symbol>
            </field>
[!ENDIF!][!//
[!ENDMACRO!][!/*

*/!][!MACRO "CalcLogicalPortGroups"!][!//
[!VAR "LogicalPortIdx" = "count(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitchPortGroup/*/EthIfPortRef/*)"!][!//
[!LOOP "node:order(node:refs(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*/EthIfSwitchRef), 'EthSwtIdx')"!][!//
[!VAR "BeginIdx" = "$LogicalPortIdx"!][!//
[!LOOP "node:order(./EthSwtPort/*, 'EthSwtPortIdx')"!][!//
[!VAR "LogicalPortIdx" = "$LogicalPortIdx + 1"!][!//
[!ENDLOOP!][!//
          <field>
            <field>
              <int>[!"num:i($BeginIdx)"!]</int>
            </field>
            <field>
              <int>[!"num:i($LogicalPortIdx - $BeginIdx)"!]</int>
            </field>
            <field>
              <symbol>ETHIF_REFTYPE_SWITCH</symbol>
            </field>
        </field>
[!ENDLOOP!][!//
[!ENDMACRO!][!/*

*/!][!MACRO "CalcPortsLogicalPortGroups"!][!//
[!FOR "i" = "0" TO "count(node:refs(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfSwitch/*/EthIfSwitchRef)/EthSwtPort/*) - 1"!][!//
          <field>
            <int>[!"$i"!]</int>
          </field>
[!ENDFOR!][!//
[!ENDMACRO!][!/*


*/!][!ENDIF!][!// ETHIF_PBCFG_M
