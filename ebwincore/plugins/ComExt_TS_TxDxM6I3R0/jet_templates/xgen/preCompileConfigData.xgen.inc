directory="include" file="<%= MODULE_NAME + GENERATED_FILE_NAME_SEPARATOR %>Types.h" type="types-h"?>

            <parameter name="relocatable-enable">
                <boolean-value><%= model.isComRelocatableCfgEnable() ? "true" : "false" %></boolean-value>
            </parameter>


        <!-- PreCompile type definitions -->
        <!-- START GENERATED PreCompile type definitions -->

<%
        cdsGenFile.type( comPBCfgDataModel.Com_RootConfig, true );
%>
        <!-- END GENERATED PreCompile type definitions -->
