directory="include" file="<%= MODULE_NAME + GENERATED_FILE_NAME_SEPARATOR %>CfgTypes.h" type="types-h"?>

            <parameter name="relocatable-enable">
                <boolean-value><%= preCompileData.isRelocatableConfigEnabled() ? "true" : "false" %></boolean-value>
            </parameter>


        <!-- PreCompile type definitions -->
        <!-- START GENERATED PreCompile type definitions -->
<%
        cdsGenFile.type(postBuildData.getLayout().RootCfg, true);

        SoAd_PBRAMLayoutType ramLayout = postBuildData.getDummyMemoryLayout();
        //we generate all members except the soConTxData because it contains a pointer - therefore it is generated in the SoAd_Types.h
        cdsGenFile.type(ramLayout.routingGroupStateBitMap, true);
        cdsGenFile.type(ramLayout.routingGroupTriggeredBitMap, true);
        cdsGenFile.type(ramLayout.soConRxData, true);
        cdsGenFile.type(ramLayout.soConState, true);
        cdsGenFile.type(ramLayout.soConGroupState, true);
        cdsGenFile.type(ramLayout.soConTxTcpData, true);
        cdsGenFile.type(ramLayout.pduTxData, true);
        cdsGenFile.type(ramLayout.pduTxTcpData, true);
        cdsGenFile.type(ramLayout.localIpAddrStateBitMap, true);
        cdsGenFile.type(ramLayout.listenSocketSoConMap, true);
        cdsGenFile.type(ramLayout.socketSoConMap, true);
        cdsGenFile.type(ramLayout.soConToSocketRouteDestMap,true);
        cdsGenFile.type(ramLayout.buffers, true);
        cdsGenFile.type(ramLayout.triggerTransmitBuffer, true);
        cdsGenFile.type(ramLayout.soConTxBufferState, true);
        cdsGenFile.type(ramLayout.soConTransCondChangeBitMap, true);
        cdsGenFile.type(ramLayout.notIdleBitMap, true);
%>
        <!-- END GENERATED PreCompile type definitions -->


        <!-- START static PreCompile type definitions -->
        <type name="SoAd_SoAdBufferType">
            <reference type="uint8">
                <compiler-abstraction>
                    <var memory-class="TYPEDEF"/>
                </compiler-abstraction>
            </reference>
        </type>

        <type name="SoAd_SoConModeType">
            <comment> * \brief Socket connection states</comment>
            <reference type="uint8">
                <compiler-abstraction>
                    <var memory-class="TYPEDEF"/>
                </compiler-abstraction>
            </reference>
        </type>

        <symbol-def name="SOAD_SOCON_OFFLINE" suffix="U">
            <integer-value>0</integer-value>
        </symbol-def>

        <symbol-def name="SOAD_SOCON_RECONNECT" suffix="U">
            <integer-value>1</integer-value>
        </symbol-def>

        <symbol-def name="SOAD_SOCON_ONLINE" suffix="U">
            <integer-value>2</integer-value>
        </symbol-def>
        <!-- END static PreCompile type definitions -->
